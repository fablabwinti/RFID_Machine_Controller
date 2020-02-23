//handling functions for configuration webpage residing in SPIFFS

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
  ESP.wdtFeed(); //kick hardware watchdog
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


/*
  non-working file upload through html form (may be fucked up by jquery? it works on non-jquery webpage)
  //file upload example https://tttapa.github.io/ESP8266/Chap12%20-%20Uploading%20to%20Server.html

  void handleFileUpload() { // upload a new file to the SPIFFS (key file)
  static fs::File uploadFile; //file to write to during file upload
  Serial.println(F("Upload"));
  if (server.uri() != "/key") {
    Serial.println(F("uri error"));
    return;
  }

  ESP.wdtFeed(); //kick hardware watchdog

  HTTPUpload& upload = server.upload();
  Serial.print(F("total size: "));
  Serial.println(upload.totalSize);
  Serial.print(F("chunk size: "));
  Serial.println(upload.currentSize);
  if (upload.status == UPLOAD_FILE_START) {
    Serial.print(F("Upload Name: ")); Serial.println(upload.filename);
    uploadFile = SPIFFS.open(KEYFILE, "w"); //open the key file, overwrite if it exists, create it if not
  }
  if (upload.status == UPLOAD_FILE_WRITE) {

    if (uploadFile)
    {
      if (upload.totalSize + upload.currentSize <= MAXKEYSIZE + 200)
      {
        uploadFile.write(upload.buf, upload.currentSize);
      }
      else
      {
        server.send(500, "text/plain", "File too large! Selected wrong file?");
        Serial.println(F("error: File too large"));
        uploadFile.close();
        //delete the key file:
        SPIFFS.remove(KEYFILE);
      }
    }
    else
    {
      server.send(500, "text/plain", "500: couldn't create file");
    }
  }
  if (upload.status == UPLOAD_FILE_END) {
    if (uploadFile)
    {
      uploadFile.close();
      Serial.println("Key Upload Successful");
      server.send(201, "text/plain", "File uploaded successfully");
      server.send(200, "text/plain", "File uploaded successfully");

    } else {
      server.send(500, "text/plain", "500: couldn't create file");
    }
  }
  }*/

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
  ESP.wdtFeed(); //kick hardware watchdog
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.println(F("Disconnected!"));
      websocket_connected--;
      break;
    case WStype_CONNECTED:
      {

        websocket_connected++;
        IPAddress ip = webSocket.remoteIP(num);
        // Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
        Serial.println(F("Client Connected"));
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
        else if (text == "reboot")  //reboot requested
        {
          //disconnect the webservice, then reboot
          server.stop();
          delay(100);
          WiFi.disconnect(true);
          delay(100);
          ESP.restart();  // reboot
        }
        else if (text == "programtag")  //program blank tags
        {
          RFIDtagprogrogramming = 1;
        }
        else if (text == "cleartag")  //blank out programmed tags
        {
          RFIDtagprogrogramming = 2;
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
          display.println(F("Upgrading Firmware... "));
          display.display();
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
          display.println(F("Updating SPIFFS... "));
          display.display();
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
          DynamicJsonDocument wifidata(150);//crate a buffer
          DeserializationError jsonerror = deserializeJson(wifidata, text); //parse the text
          serializeJsonPretty(wifidata, Serial); //debug!!!

          if (!jsonerror) //no error
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
          DynamicJsonDocument ipsettings(150);//crate a buffer
          DeserializationError jsonerror = deserializeJson(ipsettings, text); //parse the text
          serializeJsonPretty(ipsettings, Serial);//debug!!!
          if (!jsonerror) //no error
          {
            if (ipsettings.containsKey("IP"))
            {
              config.IP.fromString(ipsettings["IP"].as<String>());
            }
            if (ipsettings.containsKey("MASK"))
            {
              config.Netmask.fromString(ipsettings["MASK"].as<String>());
            }
            if (ipsettings.containsKey("GATE"))
            {
              config.Gateway.fromString(ipsettings["GATE"].as<String>());
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
                WS_print(ipsettings["IP"].as<String>());
                WS_print(F("\r\nNetmask = "));
                WS_print(ipsettings["MASK"].as<String>());
                WS_print(F("\r\nGateway = "));
                WS_print(ipsettings["GATE"].as<String>());
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
          DynamicJsonDocument nodesettings(150);//crate a buffer
          DeserializationError jsonerror = deserializeJson(nodesettings, text); //parse the text
          serializeJsonPretty(nodesettings, Serial); //debug!!!
          if (!jsonerror) //no error
          {
            if (nodesettings.containsKey("NDC_MNAME"))
            {
              config.MachineName = nodesettings["NDC_MNAME"].as<String>();
              config.DeviceName = config.MachineName; //also set accesspoint name
            }
            if (nodesettings.containsKey("NDC_MID"))
            {
              config.mid = nodesettings["NDC_MID"];
            }
            if (nodesettings.containsKey("NDC_SADD"))
            {
              config.serverAddress = nodesettings["NDC_SADD"].as<String>();
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
        else if (text.indexOf("RFIDKEY") != -1) //RFID card codes
        {
          //RFID access settings
          DynamicJsonDocument RFIDcodes(80);//crate a buffer
          DeserializationError jsonerror = deserializeJson(RFIDcodes, text); //parse the text
          serializeJsonPretty(RFIDcodes, Serial); //debug!!!
          if (!jsonerror) //no error
          {
            if (RFIDcodes.containsKey("RFIDKEY"))
            {
              //note: RFIDKEY is provided as a hex string with the leading 0x (string is 14 characters long, representing the 6 byte code, examlple: '0xDEADBEEF1234')
              //need to extract the bytes from the string
              char hexbyte[3]; //char string for one byte of hex code (plus termination)
              hexbyte[2] = 0; //null termination of char string
              String hexnumber = RFIDcodes["RFIDKEY"].as<String>();
              Serial.print("got key: (LSB first) ");
              for (i = 0; i < 6; i++)
              {
                //copy two chars to the buffer
                hexbyte[0] = hexnumber.charAt(12 - 2 * i);
                hexbyte[1] = hexnumber.charAt(13 - 2 * i);
                //convert the buffer into a integer number
                config.RFIDkey[i] = (uint8_t)strtol(hexbyte, 0, 16);
                Serial.print(config.RFIDkey[i], HEX);
                Serial.print(" ");
              }
            }
            if (RFIDcodes.containsKey("RFIDCODE"))
            {
              String code = RFIDcodes["RFIDCODE"].as<String>();
              config.RFIDcode[15] = 'X'; //debug only

              Serial.println(" ");
              Serial.print(F("got code: "));
              for (i = 0; i < 16; i++)
              {
                config.RFIDcode[i] = code.charAt(i);
                Serial.write(config.RFIDcode[i]);
              }
              Serial.println(" ");
            }
            WriteConfig();
            WS_print(F("RFID codes saved\r\n"));
          }
          else
            Serial.println(F("RFID code json parsing failed"));
        }
        else if (text.indexOf("APIKEY") != -1) //access to the web configpage
        {
          //API access settings
          //parse the json text
          DynamicJsonDocument apiconfig(150);//crate a buffer
          DeserializationError jsonerror = deserializeJson(apiconfig, text); //parse the text
          serializeJsonPretty(apiconfig, Serial); //debug!!!
          if (!jsonerror) //no error
          {
            if (apiconfig.containsKey("APIKEY"))
            {
              config.APIkey = apiconfig["APIKEY"].as<String>();
            }
            if (apiconfig.containsKey("APIUSER"))
            {
              config.APIuser = apiconfig["APIUSER"].as<String>();
            }

            WriteConfig();
            WS_print(F("API Settings saved\r\n"));
          }
          else
            Serial.println(F("api settings json parsing failed"));
        }
        else if (text.indexOf("WEBUSER") != -1)
        {
          //API access settings
          //parse the json text
          DynamicJsonDocument webaccess(150);//crate a buffer
          DeserializationError jsonerror = deserializeJson(webaccess, text); //parse the text
          serializeJsonPretty(webaccess, Serial); //debug!!!
          if (!jsonerror) //no error
          {
            if (webaccess.containsKey("WEBUSER"))
            {
              config.webUser = webaccess["WEBUSER"].as<String>();
            }
            if (webaccess.containsKey("WEBPASS"))
            {
              config.webPW = webaccess["WEBPASS"].as<String>();
            }

            WriteConfig();
            WS_print(F("Page access Settings saved\r\n"));
          }
          else
            Serial.println(F("webaccess settings json parsing failed"));
        }
        else if (text.indexOf("UID") != -1) //amdin UID as json string
        {
          //API access settings
          //parse the json text
          DynamicJsonDocument uiddata(150);//crate a buffer
          DeserializationError jsonerror = deserializeJson(uiddata, text); //parse the text
          serializeJsonPretty(uiddata, Serial); //debug!!!
          if (!jsonerror) //no error
          {
            if (uiddata.containsKey("UID"))
            {
              config.adminUID = uiddata["UID"];

              Serial.println(" ");
              Serial.print(F("got UID: "));
              Serial.println(config.adminUID);
            }

            WriteConfig();
            WS_print(F("Admin UID saved\r\n"));
          }
          else
            Serial.println(F("webaccess settings json parsing failed"));
        }
      }
        break;
      case WStype_BIN:
        {
          Serial.println(F("receiving binary WS data:"));
          Serial.write(payload, lenght);
          Serial.println("");

          static fs::File uploadFile; //file to write to
          ESP.wdtFeed(); //kick hardware watchdog
          Serial.print(F("size:"));
          Serial.println(lenght);
          uploadFile = SPIFFS.open(KEYFILE, "w"); //open the key file, overwrite if it exists, create it if not
          if (uploadFile)
          {
            if (lenght <= MAXKEYSIZE)
            {
              uploadFile.write(payload, lenght);
              WS_print(F("KEY file saved\r\n"));
            }
            else
            {
              WS_print(F("file too big"));
              Serial.println(F("File too large"));
              uploadFile.close();
              //delete the key file:
              SPIFFS.remove(KEYFILE);
            }

            uploadFile.close();

          }
          else
          {
            WS_print(F("ERROR: create failed\r\n"));
            SPIFFS.remove(KEYFILE); //remove the file if it exists (it may be corrupted?)
          }

          //USE_SERIAL.printf("[%u] get binary lenght: %u\n", num, lenght);
          // hexdump(payload, lenght);
          // send message to client
          // webSocket.sendBIN(num, payload, lenght);
        }
        break;
      }
  }
