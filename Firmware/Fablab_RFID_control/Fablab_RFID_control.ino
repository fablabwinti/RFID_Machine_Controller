
/*
  Tested on Arduino 1.8.5 using ESP8266 Arduino V2.4.0  (https://github.com/esp8266/Arduino/releases)
  For tested library versions, see next to their includes.
  Note on data structure

  -the configuration of the node resides in eeprom. this includes:
    -wifi configuration
    -machine ID, machine name, machine pricing

  -the SPIFF file system contains
    -the config web page
    -user IDs and access control stuff (in a database file)
  -logging is done on the SD card:
    -system event log files:
      -booting/rebooting/crash
      -server communication log
      -web page access

    -user login/logout is saved to a separate file
      -uid, username, transferred to server flag


  Other notes:
    -the displa must show: current user name, log time, running price
    -when not logging: current time&date, machine name, machine pricing
    -at bootup: show fablab logo or something
    -maximum number of users: 2000

 * */

//set these compile time parameters to fit your needs

#define TIMEZONE 1 //GMT +1  adjust to your timezone if needed, can be negative
#define SERVERMININTERVAL 1000  // minimum interval (in ms) between server sendouts (where a pending event is sent) default: 1000
#define MULTIWIFIS 2 //number of wifi credentials to store for wifi multi (uses lots of ram in the config!) default: 2

extern "C" {
#include "user_interface.h" //for light sleep stuff
}
#define FS_NO_GLOBALS //allow spiffs to coexist with SD card
#define FASTLED_ESP8266_RAW_PIN_ORDER
#include <FS.h> //spiff file system
#include <EDB.h> //use the fork here: https://github.com/fablabwinti/EDB
#include <SPI.h>
#include <MFRC522.h> //RFID library (V1.3.6 tested)
#include <FastLED.h> //(V3.1.6 tested)
#include <TimeLib.h>  //Time library https://github.com/PaulStoffregen/Time
#include "RtcDS3231.h" //RTC library by makuna: https://github.com/Makuna/Rtc (V2.0.2 tested)
#include <Adafruit_SSD1306.h> //oled display library (V1.1.2 tested)
#include <Adafruit_GFX.h> //(V1.2.3 tested)
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>

//#include <ESPAsyncTCP.h>
//#include <ESPAsyncWebServer.h>
#include <EEPROM.h>
#include <WiFiUdp.h>
#include <IPAddress.h>
#include <Wire.h>
#include <SD.h>
#include <ESP8266mDNS.h>  //works on mac out of the box, need to install apple's 'Bonjour' service to work on other systems
#include <WebSocketsServer.h> (V2.0.9 tested)
#include <ArduinoJson.h> // https://bblanchon.github.io/ArduinoJson/  (V5.13.0 tested)
#include "helpers.h"
#include "sound.h"
#include "LED.h"

#include "global.h"
#include "database.h"

#include "display.h"
#include "RTC.h" //real time clock and other time management
#include "NTP.h" //network time protocol management

#include "SDcard.h"

#include "WiFi.h"
#include "ServerRequests.h"
#include "output.h"

#include "webpage.h"
#include "RFID.h"




// SPI pin configuration figured out from here:
// http://d.av.id.au/blog/esp8266-hardware-spi-hspi-general-info-and-pinout/

/*  TODO
 * *******
  -add global flags for: SD state, WIFI state, Server connection state, database state 
  -if saving to SD card fails or if connection to server fails multiple times, increase the time between server connect attempts to not slow down normal operation too frequently
  -create a config section for all compile time config stuff (started putting it on beginning of this file, complete?)
  -create log file structure

  -thought: add admin key (UID) that can be updated on webpage and is not deleted on a factory reset -> admin key always works no matter what.
    -> also need to add possibility to do factory reset if program is stuck in an infinite reboot loop -> show boot screen longer while flash key is pressed during bootup, allowing the mode to change

  -add SSL to all server traffic
  -add SD card viewer to webpage
  -- 
 
  -IMPORTANT BUGFIX: in case RTC fails, the controller has to display a huge warning! also, update has to be forced more frequently!
  -optimize ram usage of local config (currently it is fully copied to ram and always kept there using over 512bytes of ram)

  -switch to async web server (more stable) -> not for now

  
  IN PROGRESS:



  DONE:
    -add more debug outputs to websocket print (see example in verifyRFIDdata() )
    -add time control for access
   -program a cache function that writes log entries to the SD card. one log file per month or something and one file for 'unsent messages' or better: create a database on the sd card for entries, add a flag and clear it if the entry was sent out succesfully. -> DONE
  -need a solid way to handle the admin key, independant of timer (in case no time is available from RTC or NTP to unlock the machine) -> DONE
   -add a config variable for the key stored in the first sector (same key for all cards), adds security to make cards impossible to be copied (easily) as access to this sector is restricted -> DONE
   -add a config key for password to access the RFID cards first sector ->DONE
   -add possibility to update spiffs through webpage -> DONE
   -add buzzer support -> DONE
   -clean out unused stuff -> DONE
   -add user database -> DONE
   -maybe: add sleep functions to save power -> still need to check the RFID module like every 100ms -> DONE
   -parse incoming json string
   -json input and output functions for database
   -make web-page access restricted by password
   -add firmware update password to eeprom -> NO, it can only be changed in the code
   -read log files on bootup, transfer flagged events to server and unflag them -> DONE
   -add RTC support -> DONE
   -add OLED support -> basic functionality: DONE
   -add RFID support -> basic functionality: DONE
   -use fastled instead of neopixel
   -add timezone and automatic daylightsaving adjustment (for europe, us has different DST rules) -> DONE
   -add timezone to EEPROM and make selectable on webconfig -> NO, must be set in code before compiling
   -put server address into eeprom, make it configurable on webpage
   -fix webpage bug: when sending DHCP settings the page does reload -> done
   -make all webpage traffic websocket based (is currently partially HTTP based which is less secure as passwords and stuff get sent through the URL and will be stored in browser history)
   -add more websocket messages throughout the code, especially in the webpage & config section
   -make the access to webpage and update more tamper-proof (like only allow it in accesspoint mode, only at bootup is not helping, the module can easily be crashed by flooding http commands)  -> need to press button to start webserver
   -webpage: add button for firmware update, add button for spiffs update
*/


void setup() {
  delay(400);  // wait for power to stabilize
  Serial.begin(115200); //comment the led init below if using serial
  Serial.println(F("\r\n\r\n*******************************"));  // todo: add actual build info here
  Serial.println(F("** Fablab Winti RFID CONTROL **"));  // todo: add actual build info here
  Serial.println(F("*******************************"));  // todo: add actual build info here
  Serial.println(F("\r\n\r\n"));  // todo: add actual build info here

  //**********************
  //FAILSAFE FACTORY RESET
  //**********************
  //press flash button immediateley after releasing the reset but not during reset (that will start the bootloader)
  forceFactoryReset(); //if button is pressed for 2 seconds, a factory reset is execuded


  //*****************
  //SOFTWARE INIT
  //*****************

  watchdog = 0;  // initialize watchdog counter (used in ticker interrupt)
  localTimeValid = false;  // set true once the time is set from NTP server
  //writeDefaultConfig();
  ReadConfig();  // read configuration from eeprom, apply default config if invalid
  // printConfig(); //debug function
  SPIFFS.begin(); //init local file system
  userDBInit();

  //*****************
  //HARDWARE INIT
  //*****************
  ConfigureWifi();  //connect to wifi
  Wire.begin(); //I2C, default pins: 4 & 5
  SPI.begin();
  displayinit(); //show bootup screen
  RTCinit(); //init the local time from RTC
  //LEDinit(); //intialize WS2812 fastled library (comment this line if using Serial debugging output)
  Serial.println("SD init");
  delay(100);
  SDmanager();  // initialize SD card if present
  SDwriteLogfile("Boot");
  initRFID();
  playBeep(); //init the beeper by playing a short sound
  initOutput(); //initialize output pin


  Serial.print(F("Free heap:"));
  Serial.println(ESP.getFreeHeap(), DEC);
  Serial.print(F("EEPROM USED:"));
  Serial.println(EE_END, DEC);


  display.println(F("setup done"));
  display.display();
  delay(800);


  //test search speed:
  Serial.println(F("full DB searchtest"));
  uint32_t starttime = millis();
  uint32_t uidtofind = 456;
  userDBfindentry(uidtofind);
  uint32_t endtime = millis();
  Serial.print(F("it took "));
  Serial.print(endtime - starttime);
  Serial.println(F("ms"));

  //print full user database:
  //userDBprintout();

  addEventToQueue(1, "" ); //send 'controller start' event
}
/*
  void sleepcallback()
  {
  Serial.println("I'm Back!");
  }
*/

long testtimestamp = 0;

void loop() {
  yield();  // run background processes
  watchdog = 0;  // kick the watchdog

  checkRFID();


  if (webserver_active == false)
  {

    displayUpdate();


    if (machineLocked) //only run wifi accessing stuff if user is logged in (wifi is not 100% stable or has long timeouts)
    {
      UpdateDBfromServer(); //update the user database if necessary
      sendPendingEvents(false);  // send data out (if available in RAM, does not check the SD card)
      timeManager(false); // check the local time
      SDmanager();  // check SD card (and send locally saved events)
    }

  }
  else //serve the config webpage (reboot to deactivate)
  {
    server.handleClient(); //handles http connections and webpage requests
    webSocket.loop();   // handles websocket requests
  }

  wifiCheckConnection();
  updateLED();  // update LED output
  checkButtonState();  // check GPIO0 button state


  delay(180); //automatically adds some sleep mode, saving power, not necessary to manually put it to sleep (only works if connected to network)

}
