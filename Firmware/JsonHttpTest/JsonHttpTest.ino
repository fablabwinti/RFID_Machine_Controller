// Sample Arduino Json Web Client
// Downloads and parse http://jsonplaceholder.typicode.com/users/1
//
// Copyright Benoit Blanchon 2014-2017
// MIT License
//
// Arduino JSON library
// https://bblanchon.github.io/ArduinoJson/
// If you like this project, please add a star!

#include <ArduinoJson.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>

const char* ssid     = "IUY-12651";
const char* password = "Dinosaurier-sind-Vegetar1er";

const char* host = "192.168.1.25";
uint16_t port = 3000; //port of server to connect to
const char* server_URI = "/machines/2/tags";       //uri to read
const unsigned long HTTP_TIMEOUT = 10000;  // max respone time from server
const size_t MAX_CONTENT_SIZE = 512;       // max size of the HTTP response

// The type of data that we want to extract from the page
struct UserData {
  char name[32];
  uint16_t tid; //tag id
  uint32_t tag_uid; //uid of the tag
  char starttime[28]; //start of validity
  char endtime[28]; //expiration date
};



void setup() {
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void loop() {
/*
  Serial.print("connecting to ");
  Serial.print(host);
  Serial.print(":");
  Serial.println(port);
*/
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    delay(1000);
    return;
  }


 // Serial.print("Requesting URI: ");
 // Serial.println(server_URI);

  // This will send the request to the server
  client.print(String("GET ") + server_URI + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  String str = client.readStringUntil('\r');
 // Serial.print(str);
  if (str.indexOf("200 OK") != -1) {
   // Serial.println(F("Server response OK"));
  }
  else //server response failed (404 or other error)
  {
    Serial.println(F("Server response FAILED"));
  }

  client.setTimeout(800);

  //now the message body is next, parse the json stream:
  //[{"tid":1,"uid":"0856898896","owner":"Claudio Prezzi","start":"2017-05-31T22:00:00.000Z","end":"2017-12-31T23:00:00.000Z"},{"tid":2,"uid":"0079830412","owner":"Admin","start":"2017-06-19T22:00:00.000Z","end":"2017-12-31T23:00:00.000Z"}]

  //the json library cannot handle big, incomplete streams so we need to divide the stram into json objects manually:

  client.readStringUntil('['); //discard the rest of the header and the inital bracket
  int entries = 0;
  while (client.available() > 0)
  {
    DynamicJsonBuffer jsonBuffer(512);
    str = "[";
    for (uint8_t i = 0; i < 10; i++)
    {
      str += client.readStringUntil('}');
      str = str + "}"; //insert discarded closing bracket
    }
    client.read(); //discard ','
    str += "]";
    //Serial.println(str);
    JsonArray& root = jsonBuffer.parseArray(str);

    if (!root.success()) {
      Serial.println("JSON parsing failed!");
      client.stop();
      return;
    }
    
    //  root.prettyPrintTo(Serial);
    //now print the parsed data:
    /*
    Serial.print("size = ");
    Serial.println(root.size());
    for (int i = 0; i < root.size(); i++)
    {
      uint16_t tid = root[i]["tid"];
      Serial.println(tid);
      Serial.println(root[i]["owner"].asString());
      uint32_t uid = root[i]["uid"];
      Serial.println(uid);
      Serial.println(root[i]["start"].asString());
      Serial.println(root[i]["end"].asString());
    }*/
    entries+= root.size();

  }
  if(entries != 499)
  {
   Serial.print("entries received = ");
   Serial.println(entries);
  }


  delay(500);
}
