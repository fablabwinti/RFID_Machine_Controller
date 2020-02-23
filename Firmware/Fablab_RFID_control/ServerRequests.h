

unsigned long serverUpdateTime = 0;  // used to control when data is sent out (must not do it faster than every x seconds)

//public key used by the server used to verify we are talking to the correct server
// Extracted by: openssl x509 -pubkey -noout -in cert.pem
//run the command in the /access_auth_server/keys folder
const char* keyfile = KEYFILE; //file uploaded to spiffs containing the public key (default is "key.pem")


/*= R"KEY(
  -----BEGIN PUBLIC KEY-----
  MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAs3OsT+I3FqRmy7QlBXd7
  mZexvdwNu9O3LlL5qTwTPDXtHTy2f5WdiFQD/U5Rpfa5wQsyrg3gdDmgx++1lllF
  4hMdN9mFPats7QGBlaRC5Z7sAkJ2dbrvH6QUSep9xUnKUBUJe/OHH1spE9kVLdU1
  ZFwslvqQcvU4IY+NGJTj35b66q1tvEwCOy0kgfL+h20rCkUanH/bq0ISLLP+ReLS
  6yt/wya8fj1lMILpITEEUd46ymhe0ABEGA16/z4g1OoV7NNUaptN+R4+YFdC/neX
  UOAjrUERJKPk+/dKFJhm46fkYD/IRcDVy+oP0VfM0tAAIqjY0kK/MIC4hNTK0seO
  GQIDAQAB
  -----END PUBLIC KEY-----
  )KEY";*/


/*
   as an alternative to using the public key the server can be verified using the fingerprint (which is less secure though)
  fingerprint:
  static const char fp[] PROGMEM = "B0:F1:AA:B0:0E:9C:40:C5:B0:C4:66:2B:4E:F1:B7:99:83:E6:3F:84";//b0f1aab00e9c40c5b0c4662b4ef1b79983e63f84

*/

bool readKeyfromSPIFFS(uint8_t* charbuffer, uint16_t buffersize)
{
  //read the key from SPIFFS file
  //if it does not exist, no connection is possible
  if (SPIFFS.exists(keyfile))
  {
    fs::File publicKeyFile;
    publicKeyFile = SPIFFS.open(keyfile, "r"); //open readonly
    if (publicKeyFile)
    {
      Serial.println(F("key file opened"));
      publicKeyFile.read((uint8_t*)charbuffer, buffersize);
      publicKeyFile.close(); //close the databasefile if it is open
      /*
            Serial.println(F("key dump:"));
            Serial.println(F("********************"));
            //print uploaded file to console

            Serial.write(charbuffer, MAXKEYSIZE);
            Serial.println(F("********************"));
      */
      return true;
    }
    else
    {
      Serial.println(F("Error opening key file"));
      return false;
    }
  }
  else
  {
    Serial.println(F("key file not found"));
    return false;
  }
}

//send all pending events to the server, save to event database (SPIFFs or SD card) in case 'saveiffail' = true, try to connect up to 10 times if 'enforce' = true (saved to database immediately if connect fails)
void sendToServer(sendoutpackage* datastruct, bool saveiffail, bool enforce) {
  // checks if we can already send more data. if we can, check if data needs to be sent.
  // then format the data and send it out through the ether

  char pubkey[MAXKEYSIZE]; //buffer for the public key

  // Use WiFiClient class to create TCP connections
  //WiFiClient client;
  if (readKeyfromSPIFFS((uint8_t*) pubkey, MAXKEYSIZE) == false) return; //read public key from spiffs (upload through webpage)



  BearSSL::WiFiClientSecure client;
  BearSSL::PublicKey key(pubkey);
  client.setKnownKey(&key);


  client.setTimeout(500); //use short timeout, the server is local and should react pretty fast if it is available, if not this reduces unnecessary long delays
  static uint8_t connectfailcounter = 0;
  static uint8_t unhealthy_delaycounter = 10; //on first run, assume the server connectin is ok

  if (WiFi.status() == WL_CONNECTED) {
    if ((millis() - serverUpdateTime > SERVERMININTERVAL) || enforce)  // do not send data more often than SERVERMININTERVAL to ease on server traffic
    {
      serverUpdateTime = millis();
      if (serverhealthy == false) //if server seems to be down, slow down the interval, if enforce is requested, immediately save to event database
      {
        if (enforce && saveiffail)
        {
          Serial.println(F("writing entry to database"));
          eventDBaddentry(datastruct); //transfer this event over to the event database (pending flag is removed there so it will not be sent from queue)
        }

        unhealthy_delaycounter++;
        if (unhealthy_delaycounter < 8)
        {
          return; //if server connection seems to be bad, only update at greater intervals
        }
        else
        {
          unhealthy_delaycounter = 0;
        }
      }

      uint8_t i;
      uint8_t needupdate = 0;
      String payload = "";
      if (datastruct->pending)  // if sendout flag is set (just double checking)
      {
        needupdate = 1;

        DynamicJsonDocument event(256);//crate a buffer
        event["timestamp"] = convertToTimestring(datastruct->timestamp); //datastruct->timestamp;
        event["mid"] = config.mid;
        event["eid"] = datastruct->event;
        if (datatosend[i].tid >= 0)
        {
          event["tid"] = datastruct->tid;
        }
        event["remarks"] = datastruct->remarks;
        serializeJson(event, payload);
      }


      if (needupdate == 0)
        return;

      Serial.println("Sendoutdata: ");

      Serial.println(payload);
      char serveradd[32];
      config.serverAddress.toCharArray(serveradd, 32);
      while (!client.connect(serveradd, config.serverPort)) { //enforces a connection
        Serial.println(F("Server connect failed"));
        delay(1); //run background stuff
        ESP.wdtFeed(); //kick hardware watchdog
        LED_blink_once(20);
        //if this fails multiple times, write data to event database
        connectfailcounter++;
        if (connectfailcounter > 3) //if connection failed three times, server connection is bad
        {
          serverhealthy = false;
        }
        if (connectfailcounter > 5)
        {
          if (saveiffail)
          {
            Serial.println(F("writing entry to database"));
            eventDBaddentry(datastruct); //transfer this event over to the event database (pending flag is removed there so it will not be sent from queue)
          }
          return;
        }
        if (enforce == false) return; //if not forced to try to resend, try again later, else, retry.
      }
      Serial.println(F("Server connect successful"));
      connectfailcounter = 0;
      //send POST to '/api/logs'
      client.print(String("POST /api/logs HTTP/1.0\r\n") +
                   "Host: " + config.serverAddress + "\r\n" +
                   "Connection: close\r\n" +
                   "Content-Type: application/json\r\n" +
                   "Content-Length: " + String(payload.length()) +
                   "\r\n\r\n");

      client.print(payload);

      int timeout = 0;
      while (client.available() == 0) {
        delay(10);
        if (timeout++ > 200)
          break;  // wait 2s max
      }
      // Read all the lines of the reply from server and print them to Serial

      String line = client.readStringUntil('\r');
      Serial.println(line);
      if (line.indexOf("200 OK") != -1) {
        datastruct->pending = 0;  // reset flag after sending
        serverhealthy = true;
#ifdef SAVE_LOG_TO_SD
        SDwriteStringToLog(payload);//write it to the SD card log as a backup measure
#endif
        Serial.println(F("Serverupdate OK"));
      }
      else
      {
        Serial.println(F("Serverupdate ERROR"));
        createErrorEvent(line); //server error, send out parts of the received error code
        if (saveiffail)
          eventDBaddentry(datastruct); //transfer this event over to the database (pending flag is removed there so it will not be sent from queue)
        LED_blink_once(20);//blink red
      }


      Serial.println();
      Serial.println(F("closing connection"));
      client.stop();
    }
  }
  else //no wifi available, log the event to SD card database for later transfer
  {
    if (saveiffail)
    {
      ESP.wdtFeed(); //kick hardware watchdog
      yield();  // run background processes
      Serial.println("no wifi, saving to database");
      eventDBaddentry(datastruct); //transfer this event over to the event database (pending flag is removed there so it will not be sent from queue)
    }
  }
}

//send pending events to the server (they are automatically transferred to the events database if sending fails)
//if 'enforce' = true the data is sent immediately or saved to event database if it fails multiple times, use enforce to make sure data is not lost (used to send login data as no wifi access is allwed after a login)
void sendPendingEvents(bool enforce)
{
  uint8_t i;
  for (i = 0; i < SERVERPACKETS; i++)  // go through all possible data structs, there can be multiple events pending
  {
    if (datatosend[i].pending)  // if sendout flag is set
    {
      yield();
      ESP.wdtFeed(); //kick hardware watchdog
      sendToServer(&datatosend[i], true, enforce); //send the data, save to event database if sending fails
      if (enforce == false)
      {
        return; //return, do the others later (to not block login events for too long)
      }
    }
  }
}

//update user database from server
void UpdateDBfromServer(void) {

  static int8_t retries = 5; //try to update 5 times, then give up
  static uint32_t timetoupdateDB = 0;

  long tick = millis();
  //try to update 5 times in interals of 10 seconds (if wifi is connected and server connection is ok)
  if (refreshUserDB && (WiFi.status() == WL_CONNECTED)  && serverhealthy && (millis() - timetoupdateDB > 10000) && (retries > 0)) {

    //note: the SPIFFS file system uses wear-leveling so flash wearout is not an issue if the update is done in reasonable intervals (like not more than once every few minutes, less is better)


    char pubkey[MAXKEYSIZE]; //buffer for the public key
    if (readKeyfromSPIFFS((uint8_t*) pubkey, MAXKEYSIZE) == false) return;
    //WiFiClient client;
    BearSSL::WiFiClientSecure client;
    BearSSL::PublicKey key(pubkey);
    client.setKnownKey(&key);

    //////////////////////////
    // USER DATABASE UPDATE //
    //////////////////////////
    Serial.println(F("Updating user database..."));
    timetoupdateDB = millis();
    retries--;
    Serial.print("connecting to ");
    Serial.print(config.serverAddress);
    Serial.print(":");
    Serial.println(config.serverPort);



    char serveradd[32];
    config.serverAddress.toCharArray(serveradd, 32);
    if (!client.connect(serveradd, config.serverPort)) {
      Serial.println("connection failed");
      return;
    }

    showCloudDownload(); //show cloud download icon on the display

    String server_URI = "/api/machines/" + String(config.mid) + "/tags";
    Serial.print("Requesting URI: ");
    Serial.println(server_URI);

    // This will send the request to the server
    client.print(String("GET ") + server_URI + " HTTP/1.1\r\n" +
                 "Host: " + config.serverAddress + "\r\n" +
                 "Connection: close\r\n\r\n");
    unsigned long timeout = millis();
    while (client.available() == 0) { //also happens if the server does not send any data, i.e. the database for this machine is empty (not sure why?)
      if (millis() - timeout > 4000) {
        Serial.println(">>> Client Timeout !");
        client.stop();
        return;
      }
    }

    String str = client.readStringUntil('\r');  //read firs line of response (contains error code)
    Serial.print(str);
    if (str.indexOf("200 OK") != -1) {
      Serial.println(F(" - Server response OK"));
    }
    else //server response failed (404 or other error)
    {
      Serial.println(F("Server response FAILED"));
      client.stop();
      return;
    }

    client.setTimeout(1000);
    prepareForDBupdate(); //prepare the database for the update (i.e. create a backup file)
    //read the message body next and parse the json stream:
    //note: the json library cannot handle big, incomplete streams so we need to divide the stram into json objects manually

    client.readStringUntil('['); //discard the rest of the header and the inital bracket
    while (client.available() > 0)
    {
      delay(1); //wait for more data and make way for background activity if needed

      str = "["; //add initial array bracket
      for (uint8_t i = 0; i < 5; i++)
      {
        str += client.readStringUntil('}');
        str = str + "}"; //insert discarded closing bracket
      }
      client.read(); //discard ','
      str += "]";  //add terminating array bracket
      //Serial.println(str);
      delay(1); //run background functions

      DynamicJsonDocument root(512);//crate a buffer
      DeserializationError jsonerror = deserializeJson(root, str); //parse the text

      if (jsonerror) {
        Serial.println("JSON parsing failed!");
        client.stop();
        {
          updateCleanup(false); //cleanup the database, unsuccessful update

          return; //parsing failed for some reason (incomplete stream?)
        }
      }

      serializeJsonPretty(root, Serial);//debug!!!
      Serial.println(".");
      ESP.wdtFeed(); //kick hardware watchdog
      delay(1); //run background functions

      for (int i = 0; i < root.size(); i++)
      {

        Serial.println(" ");

        //check if received database entry is fully defined (i.e. all required fields exist)
        //if a key does not exists it is 'null' or zero if it is an integer. the .containsKey("x") is not applicable for arrays, only for objects
        //none of the numbers can be zero (lowest tid and uid is 1, 0 is reserved for admin tag)
        uint16_t tid = root[i]["tid"];
        uint32_t uid = root[i]["uid"];
        const char* name = root[i]["name"];
        uint32_t start = root[i]["start"];
        uint32_t end = root[i]["end"];
        
        if (name && tid > 0 && uid > 0 && start > 0 && end > 0) //if name checks if the pointer is nullpointer
        {
           Serial.println(F("adding valid user entry"));
//          Serial.println(tid);
//          Serial.println(root[i]["name"].as<String>());
//          Serial.println(uid);
//          Serial.println(start);
//          Serial.println(end);
      
          delay(50); //wait for more data to arrive

          if (userDBaddentry(tid, uid, start, end, name) == false)
          {
            updateCleanup(false); //cleanup the database, unsuccessful update
            return;
          }
        }
        else
        {
          Serial.println(F("user entry invalid (JSON key missing)"));
        }
      }

    }

    /////////////////////////////
    // MACHINE SETTINGS UPDATE //
    /////////////////////////////
    //on bootup, read the machine settings from the server and save them to the config

    if (machineInfoUpdated == false) //get machine info from server, if successful, set updated to true
    {
      Serial.println(F("Updating Machine info..."));
      Serial.print("connecting to ");
      Serial.print(config.serverAddress);
      Serial.print(":");
      Serial.println(config.serverPort);

      char serveradd[32];
      config.serverAddress.toCharArray(serveradd, 32);
      if (!client.connect(serveradd, config.serverPort)) {
        Serial.println("connection failed");
        return;
      }

      String server_URI = "/api/machines/" + String(config.mid);
      Serial.print("Requesting URI: ");
      Serial.println(server_URI);

      // This will send the request to the server
      client.print(String("GET ") + server_URI + " HTTP/1.1\r\n" +
                   "Host: " + config.serverAddress + "\r\n" +
                   "Connection: close\r\n\r\n");
      unsigned long timeout = millis();
      while (client.available() == 0) { //also happens if the server does not send any data, i.e. the database for this machine is empty (not sure why?)
        if (millis() - timeout > 4000) {
          Serial.println(">>> Client Timeout !");
          client.stop();
          return;
        }
      }

      String str = client.readStringUntil('\r');  //read first line of response (contains error code)
      //Serial.print(str);
      if (str.indexOf("200 OK") != -1) {
        Serial.println(F(" - Server response OK"));
      }
      else //server response failed (404 or other error)
      {
        Serial.println(F("Server response FAILED"));
        client.stop();
        return;
      }

      client.setTimeout(1000);
      //read the message body next and parse the json stream:

      client.readStringUntil('['); //discard the rest of the header and the inital bracket

      ESP.wdtFeed(); //kick hardware watchdog
      delay(10); //wait for more data and make way for background activity if needed

      str = ""; //clear string
      str += client.readStringUntil(']'); //read full json string
      Serial.println(str);

      DynamicJsonDocument machinesettings(256);//crate a buffer

      //ReadLoggingStream loggingStream(client, Serial);
      DeserializationError jsonerror = deserializeJson(machinesettings, str);

      if (jsonerror) {
        Serial.println("JSON parsing failed!");
        Serial.println(jsonerror.c_str());
        client.stop();
        return; //parsing failed for some reason (incomplete stream?)
      }
      serializeJsonPretty(machinesettings, Serial);//debug!!!
      ESP.wdtFeed(); //kick hardware watchdog
      delay(1); //run background functions
      if (machinesettings.containsKey("name") && machinesettings.containsKey("price") && machinesettings.containsKey("period") && machinesettings.containsKey("min_periods"))
      {
        config.MachineName = machinesettings["name"].as<String>();
        config.DeviceName = config.MachineName; //also set accesspoint name
        float price = machinesettings["price"]; //read into a float
        config.mPrice =  (uint16_t)(price * 100); //price is a float, multiply by 100 to get cents
        config.mPeriod = machinesettings["period"];
        config.mMinPeriods = machinesettings["min_periods"];


        if (machinesettings.containsKey("offdelay"))
        {
          config.mSwitchoffDelay = machinesettings["offdelay"];
        }
        else
        {
          config.mSwitchoffDelay = 120; //default delay is 120 seconds
        }
        WriteConfig();
        Serial.println(F("Machine Settings updated\r\n"));
        machineInfoUpdated = true;
      }

    }
  }
  else //no wifi available
  {
    return;
  }
  Serial.println("done");
  refreshUserDB = false; //reset if update successful
  userDBupdated = true; //userDB is now up to date
  retries = 5; //reset the retries for next time
  updateCleanup(true); //cleanup the database, successful update!
  long tock = millis();
  Serial.print(F("Database update successful, entries received: "));
  Serial.println(userdatabase.count());
  Serial.print(F("Update took "));
  Serial.print(tock - tick);
  Serial.println(F("ms"));
}
