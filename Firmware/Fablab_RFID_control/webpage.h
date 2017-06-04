

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


bool handleFileRead(String path) {

  if (server.args() > 0 )  // Save Settings
  {
    uint8_t updateconfig = 1;
    uint8_t i;
    String newSSID;
    String newWifiPass;
    uint8_t addWifi = 0;

    for (i = 0; i < server.args(); i++ ) {
      Serial.println(server.argName(i));

      //Wifi configuration:
      if (server.argName(i) == "SSID") newSSID = urldecode(server.arg(i));
      if (server.argName(i) == "PASS") {
        newWifiPass = urldecode(server.arg(i));
        addWifi = 1; //config is updated in the add function
      }
      //Cayenne Setup:
      if (server.argName(i) == "CA_USER")  config.CayenneUser = server.arg(i);
      if (server.argName(i) == "CA_PASS")  config.CayennePass = server.arg(i);
      if (server.argName(i) == "CA_ID")  config.CayenneID = server.arg(i);

      if (server.argName(i) == "DHCPuse") {
        Serial.println(server.arg(i));
        if (server.arg(i) == "on") {
          config.useDHCP = true;
          Serial.println(F("using DHCP"));
        }
        else {
          config.useDHCP = false;
          Serial.println(F("NOT using DHCP"));
        }
      }
      if (server.argName(i) == "IP") {
        String address = server.arg(i);
        IPAddress ip;
        ip.fromString(address);
        config.IP = ip;
      }
      if (server.argName(i) == "MASK") {
        String address = server.arg(i);
        IPAddress ip;
        ip.fromString(address);
        config.Netmask = ip;
      }
      if (server.argName(i) == "GATE")  {
        String address = server.arg(i);
        IPAddress ip;
        ip.fromString(address);
        config.Gateway = ip;
      }
    }

    if (addWifi)
    {
      wifiAddAP(newSSID, newWifiPass);
    }
    else if (updateconfig > 0) {
      WriteConfig();
    }
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
      //memset(availablePixels,0,sizeof(availablePixels)); //clear known pixels so it updates on next client connection
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

        //cayenne setting supdate: CA_USERxxx,CA_PASSxx,CA_IDxx
        String CAsettings = "CA_USER" + config.CayenneUser + ",CA_PASS" + config.CayennePass + ",CA_ID" + config.CayenneID;
        webSocket.sendTXT(num, CAsettings);




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
          //String xVal = (text.substring(4, text.length()));
          //  WLED_program = xVal.toInt();
          //scan avalable networks and sent the SSIDs to the webpage
          Serial.println("scanning for wifi");
          int n = WiFi.scanNetworks();
          Serial.println("scan done");
          if (n == 0)
            Serial.println("no networks found");
          else
          {
            Serial.print(n);
            Serial.println(" networks found");
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
              webSocket.sendTXT(num,ssid); 
            }
          }
          Serial.println("");
          // clean up ram
          WiFi.scanDelete();
        }
        else   if (text == "resetwifi") { //delete all known wifi's
          for (i = 0; i < MULTIWIFIS ; i++)
          {
            config.ssid[i] = " ";
          }
          config.nextmultiwifitowrite = 0;
          WriteConfig();
          String SSIDs = "network"; //send empty networks string
          webSocket.sendTXT(num, SSIDs);
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



