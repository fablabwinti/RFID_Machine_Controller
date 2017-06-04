//#include <DNSServer.h>

#define APTIMEOUT (10*60000) //stop accesspoint mode after this time
#define DNS_PORT 53

ESP8266WiFiMulti wifiMulti;
uint8_t wifiConnected;
uint8_t wifiConnectFailCounter;
unsigned long wifiwatchdog = 0; //time keeper for wifi connection
unsigned long APtimestamp = 0; //time keeper for wifi connection
//DNSServer dnsServer;

/*
  void setupMDNS()
  {
  // Call MDNS.begin(<domain>) to set up mDNS to point to
  // "<domain>.local" or in this case alphanode.local
  if (!MDNS.begin("lambdanodes"))
  {
    Serial.println(F("Error setting up MDNS responder!"));
  }
  else  Serial.println(F("mDNS responder started"));
  }
*/


//host a wifi access point
void wifiHostAP()
{
  //IPAddress apIP(192, 168, 4, 1);
  WiFi.disconnect(true);
  delay(100);
  Serial.print(F("Starting access point. "));
  WiFi.mode(WIFI_AP); //accesspoint is very unstable if STA mode is on and no wifi connection is available
  WiFi.softAP(config.DeviceName.c_str(), config.DevicePW.c_str());
  //dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  //dnsServer.start(DNS_PORT, "*", apIP);

  APactive = 2; //AP is up and running
  APtimestamp = millis();
  LEDColor.h = 170; //set LED blue if accesspoint is active
  LEDColor.v = LED_MAXBRIGHTNESS; //high brightness

}

//add accesspoints saved in config to wifiMulti
void wifiConnect(void)
{
  wifiConnected = 0; //if this function is called, we are not yet or no more connected
  uint8_t i;
  Serial.println(F("Connecting to WIFI"));
  //add all known connections in config to wifi multi:
  for (i = 0; i < MULTIWIFIS ; i++)
  {
    wifiMulti.addAP(config.ssid[i].c_str(), config.wifipass[i].c_str());
    Serial.print(F("Adding Network: "));
    Serial.println(config.ssid[i]);
  }

  wifiMulti.run(); //connect to wifi (starts the network scan on first call, tries to connect on further calls (in checkconnection function)
}


void wifiAddAP(String name, String password)
{
  uint8_t success = 1;
  uint8_t i;
  for (i = 0; i < MULTIWIFIS ; i++)
  {
    if (config.ssid[i] == name) //already known,maybe an updated password?
    {
      success = 0;
      if (config.wifipass[i] != password) //updated password
      {
        config.wifipass[i] = password;
        WriteConfig();
      }
    }
  }

  if (success)
  {
    config.ssid[config.nextmultiwifitowrite] = name;
    config.wifipass[config.nextmultiwifitowrite] = password;
    config.nextmultiwifitowrite = (config.nextmultiwifitowrite + 1) % MULTIWIFIS;
    WriteConfig();
  }
}


void wifiCheckConnection()
{

  //todo: set a timeout for connecting to wifi, if connection fails repeatedly, start the AP

  if (APactive > 0)
  {
    if (((millis() - APtimestamp) > APTIMEOUT) )
    {
      WiFi.mode(WIFI_STA); //deactivate access point
      APactive = 0;
      LEDColor.h = 30; //change led color to yellow
      LEDColor.v = LED_DEFAULTBRIGTHNESS;
      ESP.restart(); //todo: this was added because the mdns service does not work after returning from AP mode, can this be fixed some other way?
    }

    if (APactive == 1) wifiHostAP(); //AP was requested, so run it
    else if (APactive == 2)
    {
      LEDColor.h = 180; //keep LED blue if accesspoint is active
      LEDColor.v = LED_MAXBRIGHTNESS; //high brightness
    }
    return; //do not handle normal wifi connection in AP mode
  }

  if (WiFi.status() == WL_CONNECTED)
  {
    wifiConnectFailCounter = 0;
    if (wifiConnected == 0) //if we were not connected before, setup services
    {
      wifiConnected = 1;
      LEDColor.h = 86; //change color of LED to green
      // setupMDNS(); //OTA service already adds MDNS
      initOTAupdate(); //start OTA update service (also adds mdns service)
      MDNS.addService("http", "tcp", 80);   // Add service to MDNS-SD
      timeManager(true); //force an NTP time update
      String logstring = getTimeString();
      logstring += "\tConnected to WIFI";
      Serial.print(logstring);
      Serial.print(" IP = ");
      Serial.println(WiFi.localIP());
      SDwriteLogfile(logstring);
    }
    ArduinoOTA.handle();
  }
  else if (APactive == 0) {
    wifiConnected = 0;
    LEDColor.h = 0; //change color of LED to red
    ledbreathe = true; //'breathe' in red to indicate no WiFi connection
    //try to reconnect every five seconds if AP is not active
    if (millis() - wifiwatchdog > 5000)
    {
      wifiConnectFailCounter++;
      IPAddress ip;
      wifiwatchdog = millis();
      switch (wifiMulti.run()) {
        case WL_CONNECTED:
          if (!config.useDHCP)
          {
            WiFi.config(config.IP, config.Gateway , config.Netmask);
          }
          ip = WiFi.localIP();
          Serial.println(F("Connected to:"));
          Serial.print(F("SSID: "));
          Serial.println(WiFi.SSID().c_str());
          Serial.print("IP: ");
          Serial.println(WiFi.localIP());
          break;
        case WL_NO_SSID_AVAIL:
          Serial.println(F("Connecting Failed AP not found"));
          break;
        default:
          Serial.println(F("WiFi Connect Failed."));
          break;
      }
    }
  }
}







void ConfigureWifi()
{
  // Serial.print("Connecting to Network: ");
  //  Serial.print(config.ssid.c_str());
  //   Serial.print("using pass ");
  //   Serial.print(config.password.c_str());
  WiFi.persistent(false); //do not write SSID and password on every wifi connect, prevents flash wearout
  wifiConnectFailCounter = 0;
  //WiFi.disconnect(true); //delete any old wifi configuration (not needed with wifi multi)
  wifiwatchdog = millis();
  WiFi.mode(WIFI_STA); //set STA mode
  wifiConnect(); //connect to wifi
}



