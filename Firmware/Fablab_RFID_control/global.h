
//#include <WiFiClientSecure.h>

#define SD_CSN_PIN 0
#define SERVERPACKETS 16        // number of server sendout packets (=buffer)

#define USE_SERIALINPUT  // uncomment to acvate serial input commands (see ackpayloads.h)
#define SERVERMININTERVAL 50  // minimum interval (in ms) between server sendouts (where all pending values in the buffer are sent)
#define DEFAULTSTRINGLENGTH 32 //strings stored in eeprom have this length by default (31 chars + termination)
#define SERVERCREDENTIALSTRINGLENGTH 55 //string length for server credentials (Cayenne credentials are up to 50 chars long)
#define MULTIWIFIS 5 //number of wifi credentials to store for wifi multi

//EEPROM definitions
#define EEPROMSIZE 1024
#define EE_SSID_ADDR 8                                              //address for SSID string
#define EE_WIFIPASS_ADDR EE_SSID_ADDR+(MULTIWIFIS*DEFAULTSTRINGLENGTH)         //address for wifi password
#define EE_NEXTWIFI_ADDR EE_WIFIPASS_ADDR+(MULTIWIFIS*DEFAULTSTRINGLENGTH)         //address for next wifi credentials to (over)write (wifi multi)
#define EE_DEVICENAME_ADDR EE_NEXTWIFI_ADDR+SERVERCREDENTIALSTRINGLENGTH //address for device name (accesspoint name, default: RFIDnode-xx)
#define EE_DEVICEPASS_ADDR EE_DEVICENAME_ADDR+DEFAULTSTRINGLENGTH   //address for device passworkd (accesspoint passworkd, default: 12345678)
#define EE_NTP_ADDR EE_DEVICEPASS_ADDR+DEFAULTSTRINGLENGTH          //address for NTP server name string
#define EE_IP_ADDR EE_NTP_ADDR+DEFAULTSTRINGLENGTH                  //address for static IP
#define EE_NETMASK_ADDR EE_IP_ADDR+4                                //address for static Netmask IP
#define EE_GATEWAY_ADDR EE_NETMASK_ADDR+4                           //address for static Gateway IP
#define EE_DHCP_ADDR EE_GATEWAY_ADDR+4                              //address for 'use DHCP' boolean
#define EE_TIMEZONE_ADDR EE_DHCP_ADDR+4                             //address for timezone, signed uint8_t
#define EE_RESERVED_ADDR EE_TIMEZONE_ADDR+1                            //address reserved

//function prototypes (todo: need to clean up the depedency mess!)
void wifiAddAP(String name, String password);
time_t getRtcTimestamp(void); 


// global variables
ESP8266WebServer server(80);  // The Webserver
ESP8266HTTPUpdateServer httpUpdater; //http firmware update
//AsyncWebServer server(80);  // The Webserver
//AsyncWebSocket ws("/ws");


WebSocketsServer webSocket = WebSocketsServer(81);
WiFiClient espClient;
// WiFiClientSecure SSLclient; //secure connections
// uint8_t* SSLspacebloackreservation;
uint8_t watchdog;      // counter for watchdog
uint8_t APactive = 0;  // is zero if AP is deactivated, 1 if active (set to 1 to launch accesspoint mode)
bool localTimeValid;
bool RTCTimeValid = false; //is set true after setting the RTC successfully
bool machineLocked = true;
uint32_t userStarttime; //timestamp at start of machine use

uint8_t websocket_connected = 0;

struct NodeConfig {
  String ssid[MULTIWIFIS];           // 31 bytes maximum
  String wifipass[MULTIWIFIS];       // 31 bytes maximum
  uint8_t nextmultiwifitowrite; //next space in array to write
  String CayenneUser;         // 50 bytes
  String CayennePass;         // 50 bytes
  String CayenneID;         // 50 bytes
  String DeviceName;     // 31 bytes maximum, name for access point
  String DevicePW;       // 31 bytes maximum, password for access point
  String ntpServerName;  // 31 bytes maximum
  IPAddress IP;
  IPAddress Netmask;
  IPAddress Gateway;
  bool useDHCP;
} config;

// Server Payload
struct sendoutpackage {
  bool pending;
  String timestamp;  // time when received
  uint8_t ID;         // ID of node
  String topic;     //topic (i.e. data name)
  String data;       // numerical data (and unit)
  String logstring; //string for SDcard and webpage logging (format: "NodeType, DataType, Data") (written to SD card in JSON format)
};

//struct for user authentication

struct userAuth {
  uint32_t uid; //4 byte uid of the RFID card
  char firstname[16]; //16byte first name of user
  char familyname[16]; //16byte family name of user
} userentry;

uint16_t currentuser=0; //database entry of current user

sendoutpackage nodeDatatosend[SERVERPACKETS];
uint8_t sendoutindex;

void WriteConfig() {
  uint8_t i;
  EEPROM.begin(EEPROMSIZE);
  Serial.println("Writing Config");
  EEPROM.write(0, 'C');
  EEPROM.write(1, 'F');
  EEPROM.write(2, 'G');


  for (i = 0; i < MULTIWIFIS ; i++)
  {
    WriteStringToEEPROM(EE_SSID_ADDR + (i * DEFAULTSTRINGLENGTH), config.ssid[i], DEFAULTSTRINGLENGTH);
    WriteStringToEEPROM(EE_WIFIPASS_ADDR + (i * DEFAULTSTRINGLENGTH), config.wifipass[i], DEFAULTSTRINGLENGTH);
  }
  EEPROM.write(EE_NEXTWIFI_ADDR, config.nextmultiwifitowrite);
  WriteStringToEEPROM(EE_NTP_ADDR, config.ntpServerName, DEFAULTSTRINGLENGTH);
  WriteStringToEEPROM(EE_DEVICENAME_ADDR, config.DeviceName, DEFAULTSTRINGLENGTH);
  WriteStringToEEPROM(EE_DEVICEPASS_ADDR, config.DevicePW, DEFAULTSTRINGLENGTH);


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
  config.DeviceName = "RFIDnode-" + macID;
  config.DevicePW = "12345678";
  config.CayenneUser = "";
  config.CayennePass = "";
  config.CayenneID = "";
  
  WriteConfig();
  Serial.println(F("Standard config applied"));
}

void ReadConfig() {
  uint8_t i;
  EEPROM.begin(EEPROMSIZE);
  Serial.println("Reading Configuration");
  if (EEPROM.read(0) == 'C' && EEPROM.read(1) == 'F' && EEPROM.read(2) == 'G') {
    Serial.println("Configurarion Found!");

    for (i = 0; i < MULTIWIFIS ; i++)
    {
      config.ssid[i] = ReadStringFromEEPROM(EE_SSID_ADDR + (i * DEFAULTSTRINGLENGTH));
      config.wifipass[i] = ReadStringFromEEPROM(EE_WIFIPASS_ADDR + (i * DEFAULTSTRINGLENGTH));
    }
    config.nextmultiwifitowrite = EEPROM.read(EE_NEXTWIFI_ADDR);
    config.ntpServerName = ReadStringFromEEPROM(EE_NTP_ADDR);
    config.DeviceName = ReadStringFromEEPROM(EE_DEVICENAME_ADDR);
    config.DevicePW = ReadStringFromEEPROM(EE_DEVICEPASS_ADDR);

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


  } else {
    Serial.println(F("Configurarion NOT FOUND!"));
    writeDefaultConfig();
  }
  EEPROM.end();
}



void checkButtonState(void) {
  static uint8_t buttonstate = 0; //set to 1 if button is pressed
  static uint32_t buttontimestamp = 0;

  // note: button is connected to SD cards CSN. when pressed, the card's SPI
  // becomes active. This is not a problem just a hint.

    // wait for any ongoing SPI transaction to finish:
    while (SPI1CMD & SPIBUSY) {
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
      if (millis() - buttontimestamp > 2000 && APactive < 2)
      {
        APactive = 1;  // activate accesspoint
        Serial.println(F("accesspoint triggered"));
      }
      if (millis() - buttontimestamp > 8000) {
        writeDefaultConfig();  //"reset to factory defaults"
        uint32_t waittimestamp = millis();
       // LED_rainbow();
       //TODO: print something on the display!
        while (millis() - waittimestamp < 3000)
        {
          updateLED();
          delay(1);
        }
        delay(100);
        ESP.restart();  // reboot
      }
    }

    // todo: possibility to add more functions, like change to default radio
    // channel or something
  
}

void printConfig(void) {
  uint8_t i;
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

}

