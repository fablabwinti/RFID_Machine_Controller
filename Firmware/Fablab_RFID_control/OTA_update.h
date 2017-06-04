void initOTAupdate(void)
{
  // Port defaults to 8266
  ArduinoOTA.setPort(8266);
  // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname("RFIDcontrol");
  // No authentication by default
  //ArduinoOTA.setPassword("abc");
  ArduinoOTA.onStart([]() {
    tkSecond.detach(); //stop ticker
    LED.setPixelColor(0, LED.Color(50, 0, 50));
    LED.show(); // show LED color
    Serial.println(F("OTA update..."));
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nOTA update finished");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
    tkSecond.attach(1, Second_Tick); //start ticker
    
  });
  ArduinoOTA.begin();

}

