

String getContentType(String filename) {
  if (server.hasArg("download")) return "application/octet-stream";
  else if (filename.endsWith(".htm")) return "text/html";
  else if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".png")) return "image/png";
  else if (filename.endsWith(".gif")) return "image/gif";
  else if (filename.endsWith(".jpg")) return "image/jpeg";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".xml")) return "text/xml";
  else if (filename.endsWith(".pdf")) return "application/x-pdf";
  else if (filename.endsWith(".zip")) return "application/x-zip";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}


bool handleHTTPRequest(String path) {

  if (server.args() > 0 )  //form submit stuff (not used anymore)
  {

  }

  Serial.println("handleFileRead: " + path);
  if (path.endsWith("/")) path += "index.htm";
  String contentType = getContentType(path);
  String pathWithGz = path + ".gz";
  if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)) {
    if (SPIFFS.exists(pathWithGz))
      path += ".gz";
    fs::File file = SPIFFS.open(path, "r");
    // Serial.println(F("sending..."));
    size_t sent = server.streamFile(file, contentType);

    Serial.print("sent: ");
    Serial.print(sent, DEC);
    Serial.print(" of ");
    Serial.print(file.size(), DEC);
    Serial.print(" at ");
    Serial.println(path);
    file.close();
    return true;
  }
  Serial.println("File not Found");
  return false;
}

//print a message to the webpage through websockets
void WS_print(const String &s)
{
  String sendoutmsg = "msg" + s; //add msg identifier in front and newline characters at the end
  if (websocket_connected)
  {
    webSocket.broadcastTXT(sendoutmsg);
  }
}


void WS_print(const __FlashStringHelper *ifsh)
{
  PGM_P p = reinterpret_cast<PGM_P>(ifsh);
  String outstring = "";
  while (1) {
    uint8_t c = pgm_read_byte(p++);
    if (c == 0) break;
    outstring += String((char)c);
  }
  WS_print(outstring);
}


void WS_println(const String &s)
{
  String sendoutmsg = s + "\r\n"; //add msg identifier in front and newline characters at the end
  WS_print(sendoutmsg);
}

//web sockets page resides in SPIFFS

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) {
  uint8_t i;
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.println("Disconnected!");
      websocket_connected--;
      break;
    case WStype_CONNECTED:
      {

        websocket_connected++;
        IPAddress ip = webSocket.remoteIP(num);
        // Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        Serial.println("Client Connected");
        //update client webpage content:

        //    String websocketStatusMessage = "H" + String(myHue) + ",S" + String(mySaturation) + ",V" + String(myValue) + ",W" + String(myWhiteLedValue); //Sends a string with the HSV and white led  values to the client website when the conection gets established
        //    webSocket.sendTXT(num, websocketStatusMessage);

        String info = String(F("Client Connected"));
        WS_println(info);

        //update the webpage content:
        String connectionstatus = "WiFi";
        if (WiFi.status() == WL_CONNECTED)
        {
          IPAddress localip = WiFi.localIP();
          connectionstatus += "OK" + WiFi.SSID() + ",IP" + localip.toString();
        }
        else
        {
          connectionstatus += "NC";
        }
        webSocket.broadcastTXT(connectionstatus);

        //send out known network accesspoints:
        String SSIDs = "network";
        for (i = 0; i < MULTIWIFIS ; i++)
        {
          if (config.ssid[i] != " ")
          {
            SSIDs += ", " + config.ssid[i];
          }
        }
        webSocket.sendTXT(num, SSIDs);

        //DHCP settings update DHCPYes,IPx.xx.xxx.x,SUBxx.xx.xx.x,GATExx.xx.xx.xx
        String DHCPsettings = "DHCP";
        if (config.useDHCP) DHCPsettings += "on";
        else DHCPsettings += "off";
        DHCPsettings += ",IP" + config.IP.toString();
        DHCPsettings += ",SUB" + config.Netmask.toString();
        DHCPsettings += ",GATE" + config.Gateway.toString();
        webSocket.sendTXT(num, DHCPsettings);

        //node settings
        String NDsettings = "NDC_MNAME" + config.MachineName + ",NDC_MID" + String(config.mid) + ",NDC_SADD" + config.serverAddress + ",NDC_PORT" + String(config.serverPort);
        Serial.println(NDsettings);
        webSocket.sendTXT(num, NDsettings);


      }
      break;
    case WStype_TEXT:
      {
        Serial.print(num);
        Serial.print(" get Text: ");
        Serial.println((char*)payload);
        //websocket_client = num;
        // send message to client
        //webSocket.sendTXT(num, "Got the message, hello from the ESP and thanks for all the fish");
        // send data to all connected clients
        // webSocket.broadcastTXT("message here");

        String text = String((char *) &payload[0]);

        if (text.substring(0, 4) == "scan") { //oder: if(text.indexOf("TEXT") >=0)  oder if(text.substring(0,4) == "text")

          //scan avalable networks and sent the SSIDs to the webpage
          //Serial.println("scanning for wifi");
          int n = WiFi.scanNetworks();
          //Serial.println("scan done");
          WS_print(F("WiFi scan done\r\n"));
          if (n == 0)
            WS_print(F("No networks found\r\n"));
          else
          {
            WS_print(String(n));
            WS_print(" networks found\r\n");
            for (int i = 0; i < n; ++i)
            {
              // Print SSID and RSSI for each network found
              Serial.print(i + 1);
              Serial.print(": ");
              Serial.print(WiFi.SSID(i));
              Serial.print(" (");
              Serial.print(WiFi.RSSI(i));
              Serial.print(")");
              Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
              String ssid = "SSID" + WiFi.SSID(i);
              webSocket.sendTXT(num, ssid);
            }
          }
          Serial.println("");
          // clean up ram
          WiFi.scanDelete();
        }
        else if (text == "resetwifi") { //delete all known wifi's
          for (i = 0; i < MULTIWIFIS ; i++)
          {
            config.ssid[i] = " ";
          }
          config.nextmultiwifitowrite = 0;
          WriteConfig();
          WS_print(F("All known WiFi connections deleted\r\n"));
          String SSIDs = "network"; //send empty networks string
          webSocket.sendTXT(num, SSIDs);
        }
        else if (text == "upgradeFW") { //download new firmware file from server
          WS_print(F("Upgrading Firmware... "));
          t_httpUpdate_return ret = ESPhttpUpdate.update("http://" + config.serverAddress + "/RFIDfirmware/firmware.bin");
          switch (ret) {
            case HTTP_UPDATE_FAILED:
              WS_print(F("HTTP_UPDATE_FAILD Error : " ));
              WS_print(ESPhttpUpdate.getLastErrorString());
              WS_print(F("\r\n"));
              Serial.printf("HTTP_UPDATE_FAILED Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
              break;
            case HTTP_UPDATE_NO_UPDATES:
              WS_print(F("HTTP_UPDATE_NO_UPDATES\r\n"));
              break;
            case HTTP_UPDATE_OK:
              WS_print(F("OK\r\n"));
              Serial.println(F("HTTP_UPDATE_OK"));
              break;
          }
        }
        else if (text == "upgradeSPIFFS") { //download new firmware file from server
          WS_print(F("Upgrading SPIFFS... "));
          t_httpUpdate_return ret = ESPhttpUpdate.updateSpiffs("http://" + config.serverAddress + "/RFIDfirmware/spiffs.bin");
          switch (ret) {
            case HTTP_UPDATE_FAILED:
              WS_print(F("HTTP_UPDATE_FAILD Error : " ));
              WS_print(ESPhttpUpdate.getLastErrorString());
              WS_print(F("\r\n"));
              break;
            case HTTP_UPDATE_OK:
              WS_print(F("OK\r\n"));
              Serial.println(F("HTTP_UPDATE_OK"));
              delay(100);
              ESP.restart();  // reboot
              break;
          }
        }

        else if (text.indexOf("SSID") != -1) //add a new wifi connection (sent as JSON, containing SSID and PASS, example {"SSID": "xxx", "PASS": "xxx"}
        {
          //parse the json text
          DynamicJsonBuffer jsonBuffer(150); //crate a buffer
          JsonObject& wifidata = jsonBuffer.parseObject(text);
          if (wifidata.success())
          {
            if (wifidata.containsKey("SSID") && wifidata.containsKey("PASS"))
            {
              String newSSID = wifidata["SSID"];
              String newWifiPass = wifidata["PASS"];
              Serial.print(F("Received new wifi credentials: "));
              Serial.println(newSSID + " " + newWifiPass);
              WS_print(F("Received new wifi credentials: "));
              WS_print(newSSID + " " + newWifiPass + "\r\n");
              wifiAddAP(newSSID, newWifiPass); //adds accesspoint to the config and saves config
              WS_print(F("Config saved, reboot to connect to Wifi\r\n"));
            }
          }
          else
            Serial.println(F("wifiadd json parsing failed"));
        }
        else if (text.indexOf("DHCP") != -1) //IP & DHCP settings (sent as JSON, containing IP, MASK, GATE and DHCP example {"IP": "192.168.1.55", "MASK": "255.255.255.0","GATE": "192.168.1.1","DHCP": "off"} (DHCP can also be "on")
        {
          //parse the json text
          DynamicJsonBuffer jsonBuffer(150); //crate a buffer
          JsonObject& ipsettings = jsonBuffer.parseObject(text);
          if (ipsettings.success())
          {
            if (ipsettings.containsKey("IP"))
            {
              config.IP.fromString(ipsettings["IP"].asString());
            }
            if (ipsettings.containsKey("MASK"))
            {
              config.Netmask.fromString(ipsettings["MASK"].asString());
            }
            if (ipsettings.containsKey("GATE"))
            {
              config.Gateway.fromString(ipsettings["GATE"].asString());
            }
            if (ipsettings.containsKey("DHCP"))
            {
              String onoff = ipsettings["DHCP"];
              if (onoff.indexOf("on"))
              {
                config.useDHCP = true;
                WS_print(F("using DHCP\r\n"));
              }
              else
              {
                config.useDHCP = false;
                WS_print(F("NOT using DHCP\r\n"));
                WS_print(F("IP = "));
                WS_print(ipsettings["IP"].asString());
                WS_print(F("\r\nNetmask = "));
                WS_print(ipsettings["MASK"].asString());
                WS_print(F("\r\nGateway = "));
                WS_print(ipsettings["GATE"].asString());
                WS_print(F("\r\n"));
              }

            }
            WriteConfig();
            WS_print(F("Advanced WiFi Settings saved\r\n"));
          }
          else
            Serial.println(F("ipsettings json parsing failed"));

        }
        else if (text.indexOf("NDC") != -1) //node settings as a json string,
        {
          //node settings
          //parse the json text
          DynamicJsonBuffer jsonBuffer(150); //crate a buffer
          JsonObject& nodesettings = jsonBuffer.parseObject(text);
          nodesettings.prettyPrintTo(Serial); //debug!!!
          if (nodesettings.success())
          {
            if (nodesettings.containsKey("NDC_MNAME"))
            {
              config.MachineName = nodesettings["NDC_MNAME"].asString();
              config.DeviceName = config.MachineName; //also set accesspoint name
            }
            if (nodesettings.containsKey("NDC_MID"))
            {
              config.mid = nodesettings["NDC_MID"];
            }
            if (nodesettings.containsKey("NDC_SADD"))
            {
              config.serverAddress = nodesettings["NDC_SADD"].asString();
            }
            if (nodesettings.containsKey("NDC_PORT"))
            {
              config.serverPort = nodesettings["NDC_PORT"];
            }
            WriteConfig();
            WS_print(F("Server and Machine Settings saved\r\n"));
          }
          else
            Serial.println(F("nodesettings json parsing failed"));


        }


      }
      break;

    case WStype_BIN:
      //USE_SERIAL.printf("[%u] get binary lenght: %u\n", num, lenght);
      // hexdump(payload, lenght);
      // send message to client
      // webSocket.sendBIN(num, payload, lenght);
      break;

  }
}



