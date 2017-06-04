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

 * */

#define FS_NO_GLOBALS //allow spiffs to coexist with SD card
#include <FS.h> //spiff file system
#include <SPI.h>
#include <MFRC522.h> //RFID library
#include <Adafruit_NeoPixel.h>
#include <TimeLib.h>  //Time library https://github.com/PaulStoffregen/Time
#include "RtcDS3231.h" //RTC library: https://github.com/Makuna/Rtc
#include <Ticker.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
//#include <ESPAsyncTCP.h>
//#include <ESPAsyncWebServer.h>
#include <EEPROM.h>
#include <WiFiUdp.h>
#include <IPAddress.h>
#include <Wire.h>
#include <SD.h>
#include <ESP8266mDNS.h>  //works on mac out of the box, need to install apple's 'Bonjour' service to work on other systems
#include <ArduinoOTA.h>
#include <WebSocketsServer.h>
#include "helpers.h"
#include "sound.h"
#include "LED.h"
#include "database.h"
#include "global.h"
#include "display.h"
#include "webpage.h"
#include "RTC.h"
#include "NTP.h"
#include "SDcard.h"
#include "timer.h"
#include "OTA_update.h"
#include "WiFi.h"
#include "SendToServer.h"
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
   -add buzzer support
   -clean out unused stuff
   -maybe: add sleep functions to save power -> still need to check the RFID module like every 100ms
   -create log file structure
   -read log files on bootup, transfer flagged events to server and unflag them

*/


void setup() {
  delay(200);  // wait for power to stabilize
  Serial.begin(115200);
  Serial.println(F("\r\n\r\nFablab Winti RFID control"));  // todo: add actual build info here

  //*****************
  //SOFTWARE INIT
  //*****************

  sendoutindex = 0;
  watchdog = 0;  // initialize watchdog counter (used in ticker interrupt)
  localTimeValid = false;  // set true once the time is set from NTP server
  ReadConfig();  // read configuration from eeprom, apply default config if invalid
  // printConfig(); //debug function
  SPIFFS.begin(); //init local file system
  databaseInit();
  //*****************
  //HARDWARE INIT
  //*****************

  Wire.begin(); //I2C, default pins: 4 & 5
  SPI.begin();
  displayinit(); //show bootup screen
  RTCinit(); //init the local time from RTC
  initOutput(); //initialize output pin7
  //LEDinit();
  SDmanager();  // initialize SD card if present
  SDwriteLogfile("Boot");
  initRFID();
  playBeep(); //init the beeper by playing a short sound
  //*****************
  //NETWORK INIT
  //*****************

  ConfigureWifi();  // connect to wifi

  server.on("/", HTTP_GET, []() {
    if (!handleFileRead("/index.htm")) server.send(404, "text/plain", "FileNotFound");
  });
  server.onNotFound([]() { // handle page not found: check if file or path is available on SPIFFS
    if (!handleFileRead(server.uri()))
      server.send(404, "text/plain", "FileNotFound");
  });

  server.begin();  // start webserver  todo: only need webserver in server mode, not in client mode so could just start it there... may save some ram and make system more stable?

  webSocket.begin(); //todo: check if this can lead to memory leaks if called multiple times
  webSocket.onEvent(webSocketEvent);
  tkSecond.attach(1, Second_Tick);  // start ticker


  Serial.print("Free heap:");
  Serial.println(ESP.getFreeHeap(), DEC);
  Serial.print("EEPROM USED:");
  Serial.println(EE_RESERVED_ADDR, DEC);



}

void loop() {
  yield();  // run background processes
  watchdog = 0;  // kick the watchdog

  checkRFID();
  displayUpdate();
  wifiCheckConnection();
  SendToServer();  // send data out
  server.handleClient(); //handles http connections and webpage requests
  webSocket.loop();   // handles websocket requests
  updateLED();  // update LED output
  checkButtonState();  // check GPIO0 button state
  timeManager(false); // check the local time
  SDmanager();  // check SD card

  delay(100);
  //todo: add light sleep here. check if local time is valid after sleep. if not, update from RTC before logging any event

}
