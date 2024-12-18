
//#include <WiFiClientSecure.h>
#include "Fonts/g9regular.h"


#define SD_CSN_PIN 0
#define SERVERPACKETS 8   // number of server sendout packets (=event packet buffer in ram)
#define MAXKEYSIZE 512 //maximum allowed size of public key (cert.pem) in bytes (should be smaller than 500 bytes, not tested for larger keys)
#define KEYFILE "/key.pem" //name of the key file

#define DEFAULTSTRINGLENGTH 32 //strings stored in eeprom have this length by default (31 chars + termination)
#define SERVERCREDENTIALSTRINGLENGTH 55 //string length for server credentials (Cayenne credentials are up to 50 chars long)


//EEPROM definitions
#define EEPROMSIZE 1024
#define EE_START 8  //staring address of data (first 8 bytes can be used as identifiers and versioning)
#define EE_SSID_ADDR  EE_START                                      //base address for wifi passwords
#define EE_WIFIPASS_ADDR EE_SSID_ADDR+(MULTIWIFIS*DEFAULTSTRINGLENGTH)     //base address for wifi passwords
#define EE_NEXTWIFI_ADDR EE_WIFIPASS_ADDR+(MULTIWIFIS*DEFAULTSTRINGLENGTH)    //address for next wifi credentials to (over)write (wifi multi)
#define EE_DEVICENAME_ADDR EE_NEXTWIFI_ADDR+SERVERCREDENTIALSTRINGLENGTH //address for device name (accesspoint name, default: RFIDnode-xx)
#define EE_DEVICEPASS_ADDR EE_DEVICENAME_ADDR+DEFAULTSTRINGLENGTH   //address for device passworkd (accesspoint passworkd, default: 12345678)
#define EE_NTP_ADDR EE_DEVICEPASS_ADDR+DEFAULTSTRINGLENGTH          //address for NTP server name string
#define EE_IP_ADDR EE_NTP_ADDR+DEFAULTSTRINGLENGTH                  //address for static IP
#define EE_NETMASK_ADDR EE_IP_ADDR+4                                //address for static Netmask IP
#define EE_GATEWAY_ADDR EE_NETMASK_ADDR+4                           //address for static Gateway IP
#define EE_DHCP_ADDR EE_GATEWAY_ADDR+4                              //address for 'use DHCP' boolean
#define EE_TIMEZONE_ADDR EE_DHCP_ADDR+4                             //address for timezone, signed uint8_t (currently not used)
//machine settings
#define EE_MID_ADDR EE_TIMEZONE_ADDR+1                              //address for machine ID, uint8_t
#define EE_MPRICE_ADDR EE_MID_ADDR+1                                //address for machine price, uint16_t
#define EE_MPERIOD_ADDR EE_MPRICE_ADDR+2                            //address for machine pricing period, uint8_t
#define EE_MMINPERIODS_ADDR EE_MPERIOD_ADDR+1                       //address for machine min. pricing periods billed, uint8_t
#define EE_MSWITCHOFFDELAY_ADDR EE_MMINPERIODS_ADDR+1               //address for machine switch-off delay, uint16_t
#define EE_MACHINENAME_ADDR EE_MSWITCHOFFDELAY_ADDR+2               //address for machine name, string
//database server settings
#define EE_SERVERIP_ADDR EE_MACHINENAME_ADDR+DEFAULTSTRINGLENGTH    //address for server IP address, 32byte string (string can be an ip like "192.168.1.1")
#define EE_SERVERPORT_ADDR EE_SERVERIP_ADDR+DEFAULTSTRINGLENGTH     //address for server port, uint16_t
//RFID settings
#define EE_RFIDKEY_ADDR EE_SERVERPORT_ADDR+2                        //address for access key for RFID data
#define EE_RFIDCODE_ADDR EE_RFIDKEY_ADDR+6     //address for access code that needs to match on RFID card
#define EE_ADMINUID_ADDR EE_RFIDCODE_ADDR+16     //address for one admin UID which always has access, once set, even a complete reset will not delete this
#define EE_WEBPASS_ADDR EE_ADMINUID_ADDR+4     //address for password for config webpage access
#define EE_WEBUSER_ADDR EE_WEBPASS_ADDR+DEFAULTSTRINGLENGTH     //address for username for config webpage access
#define EE_SERVERAPIKEY_ADDR EE_WEBUSER_ADDR+DEFAULTSTRINGLENGTH     //address for key for server API access
#define EE_SERVERAPIUSER_ADDR EE_SERVERAPIKEY_ADDR+DEFAULTSTRINGLENGTH     //address for username for server API access
//additional settings for version 1
#define EE_MMINPPRICE_ADDR EE_SERVERAPIUSER_ADDR+DEFAULTSTRINGLENGTH //address for machine min. periods price, uint16_t

#define EE_END EE_MMINPPRICE_ADDR+2                                  //address reserved

//additional config stuff needed:
/*
  -webpage login credentials
  -RFID card sector access codes
  -RFID card secret confirmation key
  -three admin UIDs (admins always have access, even after factory reset)
  -Server API user and key (two strings)
  todo: make the config use fixed string lengths as char arrays, read and write the full struct in one go making it a lot more flexible!
  can use mystring.toCharArray(config.thisstring, mytring.length()+1); to copy a string to char array and String mystring = String(config.thisstring) to convert it back
*/


//function prototypes (todo: need to clean up the header file depedency mess!)
void wifiAddAP(String name, String password);
time_t getRtcTimestamp(void);
void userDBpurge(void);
bool handleHTTPRequest(String path);
void handleFileUpload();
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght);


// global variables
Adafruit_SSD1306 display(0); //do not use reset pin
ESP8266WebServer server(80);  // The Webserver
//AsyncWebServer server(80);  // The Webserver
//AsyncWebSocket ws("/ws");

WebSocketsServer webSocket = WebSocketsServer(81);
WiFiClient espClient;
RtcDS3231<TwoWire> RTC(Wire); //DS3231 RTC clock on I2C


// WiFiClientSecure SSLclient; //secure connections
// uint8_t* SSLspacebloackreservation;

uint8_t APactive = 0;  // is zero if AP is deactivated, 1 if active (set to 1 to launch accesspoint mode)
bool refreshUserDB = true; //update the user database immediately if set to true (do so on bootup)
bool userDBupdated = false;
bool machineInfoUpdated = false; //set to true once the local machine info is updated from server(happens after bootup just before userDB is updated)
bool SDcardOK = false;
bool serverhealthy = false; //set to false if server connection fails multiple times, request time is then decreased to not hinder the usage of the controller by blocking login/logout events, set to true if server connection successful
bool localTimeValid;
bool RTCTimeValid = false; //is set true after setting the RTC successfully
bool machineLocked = true;
uint32_t userStarttime; //UNIX timestamp at start of machine use
uint32_t userStoptime; //UNIX timestamp at logout
uint32_t postlogoutmillis; //time after logout (used to not permanently having to check the RTC)

uint8_t RFIDtagprogrogramming = 0; //flag used to program blank rfid cards (flag = 1) or to blank already programmed cards (flag = 2)
uint8_t websocket_connected = 0;
bool webserver_active = false; //set true if webserver is started


struct NodeConfig {
  String ssid[MULTIWIFIS];           // 31 bytes maximum
  String wifipass[MULTIWIFIS];       // 31 bytes maximum
  uint8_t nextmultiwifitowrite; //next space in array to write
  String MachineName;         //31 bytes
  uint8_t mid;                 //machine ID
  uint16_t mPrice; //price per period in cents
  uint8_t mPeriod; //pricing period in minutes
  uint8_t mMinPeriods; //minimum periods that are billed
  uint16_t mMinPPrice; //price for the first mMinPeriods periods in cents (usually mMinPeriods*mPrice, sometimes 0)
  uint16_t mSwitchoffDelay; //delay in seconds for relay switch-off after logout
  String serverAddress;         //server address tring (can be an IP)
  uint16_t serverPort;         //port of server
  String DeviceName;     // 31 bytes maximum, name for access point
  String DevicePW;       // 31 bytes maximum, password for access point
  String ntpServerName;  // 31 bytes maximum
  IPAddress IP;
  IPAddress Netmask;
  IPAddress Gateway;
  bool useDHCP;
  uint8_t RFIDkey[6]; //access key for RFID data
  uint8_t RFIDcode[16]; //access code that needs to match on RFID card
  uint32_t adminUID; //admin UID which always has access, once set, even a complete reset will not delete this
  String webPW;       // 31 bytes maximum, password for config webpage access
  String webUser;  // 31 bytes maximum, username for config webpage access
  String APIkey;       // 31 bytes maximum, key for server API access
  String APIuser;  // 31 bytes maximum, username for server API access
} config;





// Server Payload
struct sendoutpackage {
  bool pending;
  uint32_t timestamp;  // time of event
  uint8_t event;     //event to send (0 = controller_start, 1 = controller_ok, 2 = controller_error, 3 = tag_login,4 = tag_logout)
  int16_t tid;         //tag id (set to -1 if not a tag event)
  char remarks[41]; //string for any remarks for the log (limited to 40 chars to not waste a lot of ram in the buffer, must be of constant size for database)
  //note the machine id is read from the config, it does not to be saved in the payload but is added during parsing of the payload
};

//struct for user authentication, 46byte per entry, at 500 entries: 23kB -> no problem to store in flash (but may take some time to validate a tag...read speed of flash is about 400kB/s according to a test found in the forum, so at 23kb this should take less than 100ms which is ok)

struct userAuth {
  uint16_t tagid; //2byte database tag id (used in the database as a reference to the uid)
  uint32_t uid; //4 byte uid of the RFID card
  uint32_t ts_validfrom; //entry date and time of tag
  uint32_t ts_validuntil; //expiration date and time of tag
  char name[32]; //32byte name of user
} userentry;

uint16_t currentuser = 0; //database entry of current user

sendoutpackage datatosend[SERVERPACKETS]; //buffer for packages to send out
uint8_t datatosendCursor = 0; //points to next slot after latest entry in datatosend

//function prototypes (todo: need to clean up the head file depedency mess!)
bool eventDBaddentry(sendoutpackage* evententry);

void WriteConfig() {
  uint8_t i;
  EEPROM.begin(EEPROMSIZE);
#ifdef SERIALDEBUG
  Serial.println("Writing Config");
#endif
  EEPROM.write(0, 'C');
  EEPROM.write(1, 'F');
  EEPROM.write(2, 'G');
  EEPROM.write(3, 1); // version


  for (i = 0; i < MULTIWIFIS ; i++)
  {
    WriteStringToEEPROM(EE_SSID_ADDR + (i * DEFAULTSTRINGLENGTH), config.ssid[i], DEFAULTSTRINGLENGTH);
    WriteStringToEEPROM(EE_WIFIPASS_ADDR + (i * DEFAULTSTRINGLENGTH), config.wifipass[i], DEFAULTSTRINGLENGTH);
  }
  EEPROM.write(EE_NEXTWIFI_ADDR, config.nextmultiwifitowrite);
  WriteStringToEEPROM(EE_NTP_ADDR, config.ntpServerName, DEFAULTSTRINGLENGTH);
  WriteStringToEEPROM(EE_DEVICENAME_ADDR, config.DeviceName, DEFAULTSTRINGLENGTH);
  WriteStringToEEPROM(EE_DEVICEPASS_ADDR, config.DevicePW, DEFAULTSTRINGLENGTH);


  //note: need to write the IP byte by byte as direct access to the array is not possible (IPaddress is a class)
  EEPROM.write(EE_NETMASK_ADDR, config.Netmask[0]);
  EEPROM.write(EE_NETMASK_ADDR + 1, config.Netmask[1]);
  EEPROM.write(EE_NETMASK_ADDR + 2, config.Netmask[2]);
  EEPROM.write(EE_NETMASK_ADDR + 3, config.Netmask[3]);
  EEPROM.write(EE_GATEWAY_ADDR, config.Gateway[0]);
  EEPROM.write(EE_GATEWAY_ADDR + 1, config.Gateway[1]);
  EEPROM.write(EE_GATEWAY_ADDR + 2, config.Gateway[2]);
  EEPROM.write(EE_GATEWAY_ADDR + 3, config.Gateway[3]);
  EEPROM.write(EE_IP_ADDR, config.IP[0]);
  EEPROM.write(EE_IP_ADDR + 1, config.IP[1]);
  EEPROM.write(EE_IP_ADDR + 2, config.IP[2]);
  EEPROM.write(EE_IP_ADDR + 3, config.IP[3]);
  EEPROM.write(EE_DHCP_ADDR, config.useDHCP);


  EEPROM.write(EE_MID_ADDR, config.mid);
  EEPROMWriteByteArray(EE_MPRICE_ADDR, (uint8_t*)&config.mPrice, 2); //uint16_t
  EEPROM.write(EE_MPERIOD_ADDR, config.mPeriod);
  EEPROM.write(EE_MMINPERIODS_ADDR, config.mMinPeriods);
  EEPROMWriteByteArray(EE_MSWITCHOFFDELAY_ADDR, (uint8_t*)&config.mSwitchoffDelay, 2); //uint16_t
  WriteStringToEEPROM(EE_MACHINENAME_ADDR, config.MachineName, DEFAULTSTRINGLENGTH);

  WriteStringToEEPROM(EE_SERVERIP_ADDR, config.serverAddress, DEFAULTSTRINGLENGTH);
  EEPROMWriteByteArray(EE_SERVERPORT_ADDR, (uint8_t*)&config.serverPort, 2); //uint16_t

  EEPROMWriteByteArray(EE_RFIDKEY_ADDR, (uint8_t*)&config.RFIDkey, 6); //6 bytes
  EEPROMWriteByteArray(EE_RFIDCODE_ADDR, (uint8_t*)&config.RFIDcode, 16); //16 bytes
  EEPROMWritelong(EE_ADMINUID_ADDR, config.adminUID); //4 bytes

  WriteStringToEEPROM(EE_WEBPASS_ADDR, config.webPW, DEFAULTSTRINGLENGTH);
  WriteStringToEEPROM(EE_WEBUSER_ADDR, config.webUser, DEFAULTSTRINGLENGTH);
  WriteStringToEEPROM(EE_SERVERAPIKEY_ADDR, config.APIkey, DEFAULTSTRINGLENGTH);
  WriteStringToEEPROM(EE_SERVERAPIUSER_ADDR, config.APIuser, DEFAULTSTRINGLENGTH);
  
  // version 1
  EEPROMWriteByteArray(EE_MMINPPRICE_ADDR, (uint8_t*)&config.mMinPPrice, 2); //uint16_t

  EEPROM.commit();
  EEPROM.end();
}

void writeDefaultConfig(void) {
  // DEFAULT CONFIG
  uint8_t i;
  for (i = 0; i < MULTIWIFIS ; i++)
  {
    config.ssid[i] = " ";
    config.wifipass[i] = " ";
  }
  config.nextmultiwifitowrite = 0;
  config.useDHCP = true;
  config.IP[0] = 192;
  config.IP[1] = 168;
  config.IP[2] = 1;
  config.IP[3] = 44;
  config.Netmask[0] = 255;
  config.Netmask[1] = 255;
  config.Netmask[2] = 255;
  config.Netmask[3] = 0;
  config.Gateway[0] = 192;
  config.Gateway[1] = 168;
  config.Gateway[2] = 1;
  config.Gateway[3] = 1;
  config.ntpServerName = "pool.ntp.org";

  // Use MAC address to generate a uique ID like "AlphaNode-7F89" (code snippet
  // from sparkfun example)
  uint8_t mac[WL_MAC_ADDR_LENGTH];
  WiFi.softAPmacAddress(mac);
  String macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX);
  macID.toUpperCase();
  config.DeviceName = "RFIDnode-" + macID; //todo: update this to machine name once it is known
  config.DevicePW = "12345678";
  config.MachineName = "NoName";
  config.mid = 255;
  config.mPrice = 100; //price per period in cents
  config.mPeriod = 15; //pricing period in minutes
  config.mMinPeriods = 1; //minimum periods that are billed
  config.mMinPPrice = config.mMinPeriods * config.mPrice; //price for the first mMinPeriods periods in cents
  config.mSwitchoffDelay = 0; //delay in seconds for relay switch-off after logout

  config.serverAddress = "";
  config.serverPort = 3003;

  config.webPW = "admin";
  config.webUser = "admin";
  config.APIkey = "";
  config.APIuser = "";

  //note: do not overwrite the admin UID nor the access codes for the RFID card so a reset does not require to set them again

  WriteConfig();
#ifdef SERIALDEBUG
  Serial.println(F("Standard config applied"));
#endif
}

void ReadConfig() {
  uint8_t i;
  EEPROM.begin(EEPROMSIZE);
#ifdef SERIALDEBUG
  Serial.println("Reading Configuration");
#endif
  if (EEPROM.read(0) == 'C' && EEPROM.read(1) == 'F' && EEPROM.read(2) == 'G') {
    uint8_t version = EEPROM.read(3);
    // version 0 did not write a version number, but it's likely to be 0xFF from an erased flash
    if (version == 0xFF) version = 0;
#ifdef SERIALDEBUG
    Serial.println(String("Configuration version ") + version + " found!");
#endif

    for (i = 0; i < MULTIWIFIS ; i++)
    {
      config.ssid[i] = ReadStringFromEEPROM(EE_SSID_ADDR + (i * DEFAULTSTRINGLENGTH));
      config.wifipass[i] = ReadStringFromEEPROM(EE_WIFIPASS_ADDR + (i * DEFAULTSTRINGLENGTH));
    }
    config.nextmultiwifitowrite = EEPROM.read(EE_NEXTWIFI_ADDR);
    config.ntpServerName = ReadStringFromEEPROM(EE_NTP_ADDR);
    config.DeviceName = ReadStringFromEEPROM(EE_DEVICENAME_ADDR);
    config.DevicePW = ReadStringFromEEPROM(EE_DEVICEPASS_ADDR);

    //note: need to read the IPs byte by byte as direct access to the array is not possible (IPaddress is a class)
    config.Netmask[0] = EEPROM.read(EE_NETMASK_ADDR);
    config.Netmask[1] = EEPROM.read(EE_NETMASK_ADDR + 1);
    config.Netmask[2] = EEPROM.read(EE_NETMASK_ADDR + 2);
    config.Netmask[3] = EEPROM.read(EE_NETMASK_ADDR + 3);
    config.Gateway[0] = EEPROM.read(EE_GATEWAY_ADDR);
    config.Gateway[1] = EEPROM.read(EE_GATEWAY_ADDR + 1);
    config.Gateway[2] = EEPROM.read(EE_GATEWAY_ADDR + 2);
    config.Gateway[3] = EEPROM.read(EE_GATEWAY_ADDR + 3);
    config.IP[0] = EEPROM.read(EE_IP_ADDR);
    config.IP[1] = EEPROM.read(EE_IP_ADDR + 1);
    config.IP[2] = EEPROM.read(EE_IP_ADDR + 2);
    config.IP[3] = EEPROM.read(EE_IP_ADDR + 3);
    config.useDHCP = EEPROM.read(EE_DHCP_ADDR);

    config.mid = EEPROM.read(EE_MID_ADDR);
    EEPROMReadByteArray(EE_MPRICE_ADDR, (uint8_t*)&config.mPrice, 2); //uint16_t
    config.mPeriod = EEPROM.read(EE_MPERIOD_ADDR);
    config.mMinPeriods = EEPROM.read(EE_MMINPERIODS_ADDR);
    EEPROMReadByteArray(EE_MSWITCHOFFDELAY_ADDR, (uint8_t*)&config.mSwitchoffDelay, 2); //uint16_t
    config.MachineName = ReadStringFromEEPROM(EE_MACHINENAME_ADDR);

    config.serverAddress = ReadStringFromEEPROM(EE_SERVERIP_ADDR);
    EEPROMReadByteArray(EE_SERVERPORT_ADDR, (uint8_t*)&config.serverPort, 2); //uint16_t

    EEPROMReadByteArray(EE_RFIDKEY_ADDR, (uint8_t*)&config.RFIDkey, 6); //6 bytes
    EEPROMReadByteArray(EE_RFIDCODE_ADDR, (uint8_t*)&config.RFIDcode, 16); //16 bytes
    config.adminUID = EEPROMReadlong(EE_ADMINUID_ADDR); //4 bytes


    config.webPW = ReadStringFromEEPROM(EE_WEBPASS_ADDR);;
    config.webUser = ReadStringFromEEPROM(EE_WEBUSER_ADDR);;
    config.APIkey = ReadStringFromEEPROM(EE_SERVERAPIKEY_ADDR);;
    config.APIuser = ReadStringFromEEPROM(EE_SERVERAPIUSER_ADDR);;
    
    if (version >= 1) {
      EEPROMReadByteArray(EE_MMINPPRICE_ADDR, (uint8_t*)&config.mMinPPrice, 2); //uint16_t
      if (config.mMinPPrice == 0xFFFF) {
        // probably version wasn't actually 1 and we're reading uninitialized flash
        config.mMinPPrice = config.mMinPeriods * config.mPrice;
      }
    }
    else {
      config.mMinPPrice = config.mMinPeriods * config.mPrice;
    }

  } else {
#ifdef SERIALDEBUG
    Serial.println(F("Configurarion NOT FOUND!"));
#endif
    writeDefaultConfig();
  }
  EEPROM.end();
}

//start webserver to serve config page
void WebServerinit(void)
{


  webserver_active = true;
  display.clearDisplay();
  display.setFont(&g9regularFont);
  display.setCursor(0, 7);
  display.println(F("Starting Webserver"));
  if (WiFi.status() == WL_CONNECTED)
  {
    display.print(F("IP: "));
    display.println(WiFi.localIP());
  }
  else
  {
    display.println(F("WIFI DISCONNECTED"));
  }
  display.println(F("Reboot when done"));
  display.display();

  server.on("/", HTTP_GET, []() {
    if (!server.authenticate(config.webUser.c_str(), config.webPW.c_str()))
      return server.requestAuthentication();
    if (!handleHTTPRequest("/index.htm")) server.send(404, "text/plain", "FileNotFound");
  });

  server.on("/test", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "text/html", "<form method='POST' action='/upload' enctype='multipart/form-data'><input type='file' name='update'><input type='submit' value='Update'></form>");
  });

  //note: upload through form did not work, changed it to websocket binary upload
  //  server.onFileUpload(handleFileUpload);
  //  //post to '/upload' means the webpage is uploading a new public key for the server
  //  server.on("/key", HTTP_POST, []() {
  //    server.send(200);                      // Send status 200 (OK) to tell the client we are ready to receive
  // #ifdef SERIALDEBUG
  //    Serial.println("file upload ready");
  //#endif
  //  });                                  // Receive and save the key file


  server.onNotFound([]() { // handle page not found: check if file or path is available on SPIFFS
    if (!handleHTTPRequest(server.uri()))
      server.send(404, "text/plain", "FileNotFound");
  });

  server.begin();  // start webserver
  webSocket.begin(); //todo: check if this can lead to memory leaks if called multiple times
  webSocket.onEvent(webSocketEvent);


}

//call this function and it performs a factory reset, writing the default config
//caution: it can brick the webpage if eeprom size was changed for example
void forceFactoryReset(void)
{
  uint8_t buttoncounter = 0;
  // wait for any ongoing SPI transaction to finish:
  while (SPI1CMD & SPIBUSY) {
  }

  // now make the pin an input, read it and set it to an output again
  pinMode(SD_CSN_PIN, INPUT);
  while (digitalRead(SD_CSN_PIN) == LOW) {
    delay(50);
    buttoncounter++;
    if (buttoncounter > 40) //after 2 seconds, do a factory reset
    {
      writeDefaultConfig();  //"reset to factory defaults"
      userDBpurge(); //also delete the user database
      playBeep(); //to init the beeper (this function is intended to be run at the very beginning of the setup as a safety measure)
      delay(100);
      playMarch();
      delay(1000);
      ESP.restart();  // reboot
    }
  }
}

bool checkButtonState(void) {
  static uint8_t buttonstate = 0; //set to 1 if button is pressed
  static uint32_t buttontimestamp = 0;

  // note: button is connected to SD cards CSN. when pressed, the card's SPI
  // becomes active. This is not a problem just a hint.

  // wait for any ongoing SPI transaction to finish:
  uint32_t timeout = 0;
  ESP.wdtFeed(); //kick hardware watchdog
  while (SPI1CMD & SPIBUSY) {
    timeout++;
    if (timeout > 2000) break; //make sure this cannot become an infinite loop
    delay(1);
  }

  // now make the pin an input, read it and set it to an output again
  pinMode(SD_CSN_PIN, INPUT);
  if (digitalRead(SD_CSN_PIN) == LOW) {
    if (buttonstate == 0)
    {
      buttonstate = 1;
      buttontimestamp = millis();
    }
  } else
  {
    buttonstate = 0;
  }
  pinMode(SD_CSN_PIN, OUTPUT);
  digitalWrite(SD_CSN_PIN, HIGH);
  if (buttonstate == 1)
  {
    if (webserver_active == false)
    {
      WebServerinit();
    }
    if (millis() - buttontimestamp > 2000 && APactive < 2)
    {
      APactive = 1;  // activate accesspoint
#ifdef SERIALDEBUG
      Serial.println(F("accesspoint triggered"));
#endif
      display.println(F("Accesspoint Started:"));
      display.println(config.DeviceName);
      display.print(F("Pass: "));
      display.println(config.DevicePW);
      display.println(F("Go to 192.168.4.1"));
      display.display();
    }
    if (millis() - buttontimestamp > 6000) {
      //used as secondary version for factory reset
      forceFactoryReset();
      playMarch();
      delay(200);
    }
  }

  return buttonstate;

}


void printConfig(void) {
  uint8_t i;
#ifdef SERIALDEBUG
  Serial.println("config content:");
  Serial.println(config.useDHCP);
  Serial.println(config.IP[0]);
  Serial.println(config.IP[1]);
  Serial.println(config.IP[2]);
  Serial.println(config.IP[3]);
  Serial.println(config.Netmask[0]);
  Serial.println(config.Netmask[1]);
  Serial.println(config.Netmask[2]);
  Serial.println(config.Netmask[3]);
  Serial.println(config.Gateway[0]);
  Serial.println(config.Gateway[1]);
  Serial.println(config.Gateway[2]);
  Serial.println(config.Gateway[3]);
  for (i = 0; i < MULTIWIFIS ; i++)
  {
    Serial.println(config.ssid[i]);
    Serial.println(config.wifipass[i]);
  }
  Serial.println(config.ntpServerName);
  Serial.println(config.DeviceName);
  Serial.println(config.DevicePW);
  //  Serial.println(config.APIkey);
#endif
}



//function to generate a log event from data passed to function, if the queue is full, write the new event to the SD card. remarks must be smaller than 40 chars or it gets truncated
void addEventToQueue(uint8_t logevent, int16_t tagID, String remarkstr)
{
  sendoutpackage* event = &datatosend[datatosendCursor];
  if (event->pending)
  {
    // buffer full, drop the oldest entry into the database
    eventDBaddentry(event);
  }
  datatosendCursor = (datatosendCursor + 1) % SERVERPACKETS;

#ifdef SERIALDEBUG
  Serial.print(F("Event "));
#endif
  //fill in the data, add current time for timestamp
  event->pending = true;
  event->timestamp = getRtcTimestamp(); //get current timestamp from the RTC (returns 0 if RTC time is invalid due to hardware fault)
  //Serial.println(event->timestamp);
  event->event = logevent; //event to send (0 = controller_start, 1 = controller_ok, 2 = controller_error, 3 = tag_login,4 = tag_logout)
  event->tid = tagID;
  remarkstr.toCharArray(event->remarks, 41); //copy the string

#ifdef SERIALDEBUG
  Serial.println(F("queued"));
#endif
}

void addEventToQueue(uint8_t logevent, String remarkstr)
{
  addEventToQueue(logevent, -1, remarkstr); //if no tid provided, tid is set negative (and not sent out)
}

sendoutpackage* findFirstEventInQueue()
{
  uint8_t i = datatosendCursor;
  do
  {
    if (datatosend[i].pending)
    {
      return &datatosend[i];
    }
    i = (i + 1) % SERVERPACKETS;
  } while (i != datatosendCursor);
  return NULL;
}


//function to generate an error event, errorstr must be smaller than 40 chars or it gets truncated
void createErrorEvent(String errorstr)
{
  addEventToQueue(2, errorstr); //event  2 = controller_error
}
