
//todo: read this stuff from the config
const char* server_addr = "192.168.1.25";
uint16_t port = 3000;
const char* server_URI = "/machines/2/tags"; //todo: add real uri
const char* server_postURI = "/logs"; //todo: add real uri
unsigned long serverUpdateTime = 0;  // used to control when data is sent out (must not do it faster than every x seconds)



//send all pending events to the server
void sendToServer(sendoutpackage* datastruct) {
  // checks if we can already send more data. if we can, check if data needs to be sent.
  // then format the data and send it out through the ether

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  static uint8_t connectfailcounter = 0;
  static bool serverhealthy = 1; //set to false if server connection fails multiple times, request time is then decreased to not hinder the usage of the controller by blocking login/logout events

  if (WiFi.status() == WL_CONNECTED) {
    if (millis() - serverUpdateTime > SERVERMININTERVAL)  // do not send data more often than SERVERMININTERVAL to ease on server traffic (all pending data is sent in one call)
    {
      serverUpdateTime = millis();
      uint8_t i;
      uint8_t needupdate = 0;
      String payload = "";
      if (datastruct->pending)  // if sendout flag is set (just double checking)
      {
        needupdate = 1;
        DynamicJsonBuffer jsonBuffer(256); //crate a buffer for one event
        JsonObject& event = jsonBuffer.createObject();
        // event["timestamp"] = datastruct.timestamp; //todo: server does not accept unix timestamp...
        event["mid"] = 3; //todo: read this from config
        event["event"] = datastruct->event;
        if (datatosend[i].tid > 0)
        {
          event["tid"] = datastruct->tid;
        }
        event["remarks"] = datastruct->remarks;
        event.printTo(payload);
      }


      if (needupdate == 0)
        return;

      Serial.println("Sendoutdata: ");

      Serial.println(payload);

      if (!client.connect(server_addr, port)) {
        Serial.println(F("Server connect failed"));
        LED_blink_once(20);
        //if this fails multiple times, write data to SD database
        connectfailcounter++;
        if (connectfailcounter > 10)
        {
          //todo: write this event to the database and remove pending flag
        }
        return;
      }

      connectfailcounter = 0;

      client.print(String("POST /logs HTTP/1.1\r\n") +
                   "Host: http://" + server_addr + "/\r\n" +
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

        Serial.println(F("Serverupdate OK"));
      }
      else
      {
        Serial.println(F("Serverupdate ERROR"));
        //todo: add an error event to the pending message list here, add the 'line' as a comment
        //todo: write this event to the database and remove pending flag

        LED_blink_once(20);
      }


      Serial.println();
      Serial.println(F("closing connection"));
      client.stop();
    }
  }



}

//send pending events to the server (they are automatically transferred to the SD card database if sending fails)
void sendPendingEvents(void)
{
  uint8_t i;
  for (i = 0; i < SERVERPACKETS; i++)  // go through all possible data structs, there can be multiple events pending
  {
    if (datatosend[i].pending)  // if sendout flag is set
    {
      sendToServer(&datatosend[i]);
      return; //return, do the others later (to not block login events for too long)
    }
  }
}
//update user database from server
void UpdateDBfromServer(void) {

  static uint8_t retries = 10; //try to update 10 times, then give up
  static uint32_t timetoupdateDB = 0;

  long tick = millis();
  //try to update 10 times in interals of 10 seconds
  if (refreshUserDB && WiFi.status() == WL_CONNECTED  && millis() - timetoupdateDB > 10000 && retries > 0) {
    Serial.println(F("Updating user database..."));
    timetoupdateDB = millis();
    retries--;
    Serial.print("connecting to ");
    Serial.print(server_addr);
    Serial.print(":");
    Serial.println(port);

    // Use WiFiClient class to create TCP connections
    WiFiClient client;
    if (!client.connect(server_addr, port)) {
      Serial.println("connection failed");
      return;
    }

    Serial.print("Requesting URI: ");
    Serial.println(server_URI);

    // This will send the request to the server
    client.print(String("GET ") + server_URI + " HTTP/1.1\r\n" +
                 "Host: " + server_addr + "\r\n" +
                 "Connection: close\r\n\r\n");
    unsigned long timeout = millis();
    while (client.available() == 0) {
      if (millis() - timeout > 5000) {
        Serial.println(">>> Client Timeout !");
        client.stop();
        return;
      }
    }

    String str = client.readStringUntil('\r');  //read firs line of response (contains error code)
    Serial.print(str);
    if (str.indexOf("200 OK") != -1) {
      Serial.println(F("Server response OK"));
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
      DynamicJsonBuffer jsonBuffer(512); //crate a buffer for 10 database entries (todo: could optimize ram usage if necessary by making this buffer smaller)
      str = "["; //add initial array bracket
      for (uint8_t i = 0; i < 10; i++)
      {
        str += client.readStringUntil('}');
        str = str + "}"; //insert discarded closing bracket
      }
      client.read(); //discard ','
      str += "]";  //add terminating array bracket
      //Serial.println(str);
      JsonArray& root = jsonBuffer.parseArray(str); //parse the array
      if (!root.success()) {
        Serial.println("JSON parsing failed!");
        client.stop();
        {
          updateCleanup(false); //cleanup the database, unsuccessful update
          return; //parsing failed for some reason (incomplete stream?)
        }
      }

      //  root.prettyPrintTo(Serial);
      Serial.print(".");

      for (int i = 0; i < root.size(); i++)
      {
        uint16_t tid = root[i]["tid"];
        uint32_t uid = root[i]["uid"];
        const char* owner = root[i]["owner"];
        uint32_t start = root[i]["start"];
        uint32_t end = root[i]["end"];

        /*
          Serial.println(tid);
          Serial.println(root[i]["owner"].asString());
          Serial.println(uid);
          Serial.println(start);
          Serial.println(end);
        */
        if (userDBaddentry(tid, uid, start, end, owner) == false)
        {
          updateCleanup(false); //cleanup the database, unsuccessful update
          return;
        }
      }

    }

    //to update the local databese: delete the database file and create a new one with the entries received from the server
    //the SPIFFS file system uses wear-leveling so flash wearout is not an issue if the update is done in reasonable intervals (like not more than once every few minutes, less is better)
  }
  else //no wifi available
  {
    return;
  }
  Serial.println("done");
  refreshUserDB = false; //reset if update successful
  retries = 10; //reset the retries for next time
  updateCleanup(true); //cleanup the database, successful update!
  long tock = millis();
  Serial.print(F("Database update successful, entries received: "));
  Serial.println(userdatabase.count());
  Serial.print(F("Update took "));
  Serial.print(tock - tick);
  Serial.println(F("ms"));



}

