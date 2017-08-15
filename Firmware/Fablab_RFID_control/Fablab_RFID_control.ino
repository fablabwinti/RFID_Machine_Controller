/*
  Note on data structure

  -the configuration of the node resides in eeprom. this includes:
    -wifi configuration
    -machine ID, machine name, machine pricing

  -the SPIFF file system contains (in a database file)
    -the config web page
    -user IDs and access control stuff
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

#define TIMEZONE 1 //GMT +1  adjust to your timezone if needed



extern "C" {
#include "user_interface.h" //for light sleep stuff
}
#define FS_NO_GLOBALS //allow spiffs to coexist with SD card
#define FASTLED_ESP8266_RAW_PIN_ORDER
#include <FS.h> //spiff file system
#include <EDB.h> //use the fork here: https://github.com/fablabwinti/EDB
#include <SPI.h>
#include <MFRC522.h> //RFID library
#include <FastLED.h>
#include <TimeLib.h>  //Time library https://github.com/PaulStoffregen/Time
#include "RtcDS3231.h" //RTC library: https://github.com/Makuna/Rtc
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
//#include <ESPAsyncTCP.h>
//#include <ESPAsyncWebServer.h>
#include <EEPROM.h>
#include <WiFiUdp.h>
#include <IPAddress.h>
#include <Wire.h>
#include <SD.h>
#include <ESP8266mDNS.h>  //works on mac out of the box, need to install apple's 'Bonjour' service to work on other systems
#include <WebSocketsServer.h>
#include <ArduinoJson.h> // https://bblanchon.github.io/ArduinoJson/
#include "helpers.h"
#include "sound.h"
#include "LED.h"

#include "global.h"
#include "database.h"
#include "webpage.h"
#include "display.h"
#include "RTC.h" //real time clock and other time management
#include "NTP.h" //network time protocol management

#include "SDcard.h"

#include "WiFi.h"
#include "ServerRequests.h"
#include "output.h"
#include "RFID.h"





// SPI pin configuration figured out from here:
// http://d.av.id.au/blog/esp8266-hardware-spi-hspi-general-info-and-pinout/

/*  TODO
 * *******
   -add RTC support -> DONE
   -add OLED support -> basic functionality: DONE
   -add RFID support -> basic functionality: DONE
   -use fastled instead of neopixel
   -add buzzer support -> DONE
   -clean out unused stuff -> DONE
   -add user database -> DONE
   -maybe: add sleep functions to save power -> still need to check the RFID module like every 100ms -> DONE
   -create log file structure
   -read log files on bootup, transfer flagged events to server and unflag them
   -add timezone and automatic daylightsaving adjustment (for europe, us has different DST rules) -> DONE
   -add timezone to EEPROM and make selectable on webconfig
  -add time control for access
  -switch to async web server (more stable)
  -webpage: add machine info, add button for firmware update
  -webpage: timezone
  -add firmware update password to eeprom
  -make web-page access restricted by password
  -add SSL to webpage
  -put server address into eeprom, make it configurable on webpage
  -add a #define for password to access the RFID cards first sector
  -add a #define for the key stored in the first sector (same key for all cards), adds security to make cards impossible to be copied (easily) as access to this sector is restricted
  -need a solid way to handle the master key, independant of timer (in case no time is available from RTC or NTP to unlock the machine)
  -program a cache function that writes log entries to the SD card. one log file per month or something and one file for 'unsent messages' or better: create a database on the sd card for entries, add a flag and clear it if the entry was sent out succesfully.
  -parse incoming json string
  -json input and output functions for database
  

*/


void setup() {
  delay(200);  // wait for power to stabilize
  Serial.begin(115200);
  Serial.println(F("\r\n\r\n***********************************"));  // todo: add actual build info here
  Serial.println(F("** Fablab Winti RFID CONTROL **"));  // todo: add actual build info here
  Serial.println(F("*******************************"));  // todo: add actual build info here
  Serial.println(F("\r\n\r\n"));  // todo: add actual build info here

  //*****************
  //SOFTWARE INIT
  //*****************

  sendoutindex = 0;
  watchdog = 0;  // initialize watchdog counter (used in ticker interrupt)
  localTimeValid = false;  // set true once the time is set from NTP server
  ReadConfig();  // read configuration from eeprom, apply default config if invalid
  // printConfig(); //debug function
  SPIFFS.begin(); //init local file system
  userDBInit();

  //*****************
  //HARDWARE INIT
  //*****************
  ConfigureWifi();  // connect to wifi
  Wire.begin(); //I2C, default pins: 4 & 5
  SPI.begin();
  displayinit(); //show bootup screen
  RTCinit(); //init the local time from RTC
  // LEDinit();
  SDmanager();  // initialize SD card if present
  SDwriteLogfile("Boot");
  initRFID();
  playBeep(); //init the beeper by playing a short sound
  initOutput(); //initialize output pin

  //*****************
  //NETWORK INIT
  //*****************
  display.println(F("Webserver init"));
  display.display();

  server.on("/", HTTP_GET, []() {
    if (!handleFileRead("/index.htm")) server.send(404, "text/plain", "FileNotFound");
  });
  server.onNotFound([]() { // handle page not found: check if file or path is available on SPIFFS
    if (!handleFileRead(server.uri()))
      server.send(404, "text/plain", "FileNotFound");
  });

  const char* update_path = "/firmware";
  const char* update_username = "admin";
  const char* update_password = "admin";
  httpUpdater.setup(&server, update_path, update_username, update_password);
  server.begin();  // start webserver  todo: only need webserver in server mode, not in client mode so could just start it there... may save some ram and make system more stable?
  webSocket.begin(); //todo: check if this can lead to memory leaks if called multiple times
  webSocket.onEvent(webSocketEvent);

  Serial.print("Free heap:");
  Serial.println(ESP.getFreeHeap(), DEC);
  Serial.print("EEPROM USED:");
  Serial.println(EE_RESERVED_ADDR, DEC);

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
  displayUpdate();
  wifiCheckConnection();
  sendPendingEvents();  // send data out (if available)
  updateLED();  // update LED output
  checkButtonState();  // check GPIO0 button state
  timeManager(false); // check the local time
  SDmanager();  // check SD card

  if (machineLocked) //do not handle any server requests when machine is being used (prevents crashes)
  {
    server.handleClient(); //handles http connections and webpage requests
    webSocket.loop();   // handles websocket requests
    UpdateDBfromServer(); //update the user database if necessary

  }

  delay(180); //automatically adds some sleep mode, saving power, not necessary to manually put it to sleep (only works if connected to network)

  if (millis() - testtimestamp > 30000)
  {
    //generate some random event data
    testtimestamp = millis();
    datatosend[0].pending = 1;
    datatosend[0].timestamp = getRtcTimestamp();  // time of event
    datatosend[0].event = random(4);     //event to send (0 = controller_start, 1 = controller_ok, 2 = controller_error, 3 = tag_login,4 = tag_logout)
    datatosend[0].tid = random(5);         //tag id (set to -1 if not a tag event)
    datatosend[0].remarks = "testing testing 1234. is this on?"; //string for any remarks for the log

  }
}
