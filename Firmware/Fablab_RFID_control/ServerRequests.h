

const char* mqtt_server = "mqtt.mydevices.com";//"broker.hivemq.com"; //"broker.mqttdashboard.com"; //"test.mosquitto.org";//"iot.eclipse.org"; // 
unsigned long serverUpdateTime = 0;  // used to control when data is sent out (must not do it faster than every x seconds)



void SendToServer(void) {

/*
  if (WiFi.status() == WL_CONNECTED) {
    if (millis() - serverUpdateTime > SERVERMININTERVAL)  // do not send data more often than SERVERMININTERVAL to ease on server traffic (all pending data is sent in one call)
    {

//      if (!MQTTclient.connected())
//      {
//        MQTTreconnect();
//      }
      serverUpdateTime = millis();

  
      uint8_t i;
      for (i = 0; i < SERVERPACKETS; i++)  // go through all 16 possible data structs
      {
        if (nodeDatatosend[i].pending)  // if sendout flag is set
        {
          String outtopic = "v1/" + config.CayenneUser +"/things/" + config.CayenneID + "/data/N";
          outtopic += String(nodeDatatosend[i].ID)+ "_" + nodeDatatosend[i].topic;
          String outmessage = nodeDatatosend[i].data;
          
          Serial.println("Sendoutdata: ");
          Serial.print(outtopic);
          Serial.print("   ");
          Serial.println(outmessage);
        //  MQTTclient.publish( (char*)outtopic.c_str(),  (char*)outmessage.c_str());
          nodeDatatosend[i].pending = 0;
        }
      }

    }
  }*/
}



void callback(char* topic, byte* payload, unsigned int length) {
  /*
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    }
    Serial.println();

    // Switch on the LED if an 1 was received as first character
    if ((char)payload[0] == '1') {
    digitalWrite(BUILTIN_LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
    } else {
    digitalWrite(BUILTIN_LED, HIGH);  // Turn the LED off by making the voltage HIGH
    }
  */
}

void MQTTsetup(void)
{
  /*
  MQTTclient.setServer(mqtt_server, 1883);
  MQTTclient.setCallback(callback);*/
}

