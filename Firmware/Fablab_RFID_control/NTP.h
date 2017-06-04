

//ntp stuff
#define NTP_PACKET_SIZE 48 // NTP time stamp is in the first 48 bytes of the message
IPAddress timeServerIP; // pool.ntp.org NTP server address
WiFiUDP udp; // A UDP instance to let us send and receive packets over UDP
const unsigned int UDPlocalPort = 2390;


//returns date and time string of current time, "X" if no internet time is available
String getTimeString()
{

  String datetime;

  if (localTimeValid)
  {

    char temparr[5];
    sprintf(temparr, "%02u", month()); //need a fixed length, easiest using sprintf
    String monthstr = String(temparr);
    sprintf(temparr, "%02u", day()); //need a fixed length, easiest using sprintf
    String daystr = String(temparr);
    sprintf(temparr, "%02u", hour() + 1); //need a fixed length, easiest using sprintf
    String hourstr = String(temparr);
    sprintf(temparr, "%02u", minute()); //need a fixed length, easiest using sprintf
    String minutestr = String(temparr);
    sprintf(temparr, "%02u", second()); //need a fixed length, easiest using sprintf
    String secondstr = String(temparr);

    datetime = daystr + ". " + monthstr + ". " + String(year()) + " " + hourstr + ":" + minutestr + ":" + secondstr;
  }
  else datetime = "X";

  return datetime;
}

// send an NTP request to the time server at the given address
unsigned long sendNTPpacket(IPAddress& address, uint32_t* timestamp, byte* packetBuffer)
{

  //todo: add reference to source of this code

  //set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  //Initialize values needed to form NTP request
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // send a packet requesting a timestamp:
  udp.beginPacket(address, 123); //NTP requests are to port 123
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
  *timestamp = millis();
}



//synchronize the local clock with the NTP server's
//return value is the time server roundtrip delay (below 20ms yields pretty good timestamps)
int NTP_gettime(uint32_t* t)
{
  byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets
  Serial.print(F("NTP Sync "));
  uint32_t NTP_start_time; //local timestamp to measure network roundtrip time
  uint32_t NTP_end_time; //local timestamp when data is received to measure roundtrip delay
  uint32_t timeout = millis();
  uint8_t errorcounter = 0;
  int gotresponse = 0;

  // Serial.println("Starting UDP");
  udp.begin(UDPlocalPort);
  // Serial.print("Local port: ");
  // Serial.println(udp.localPort());

  WiFi.hostByName(config.ntpServerName.c_str(), timeServerIP); //get ip for timeserver to use
  timeout = millis();
  sendNTPpacket(timeServerIP, &NTP_start_time, packetBuffer); // send an NTP packet to a time server
  // wait to see if a reply is available
  do {
    yield();
    gotresponse = udp.parsePacket();
  } while (!gotresponse && (millis() - timeout < 300));


  if (gotresponse == NTP_PACKET_SIZE) {

    NTP_end_time = millis(); //save the current millis() time at reception of packet
    // We've received a packet, read the data from it
    udp.read(packetBuffer, NTP_PACKET_SIZE); // read the packet into the buffer
    udp.stop();
    //the timestamp starts at byte 40 of the received packet and is four bytes
    unsigned long secsSince1900;
    // convert four bytes starting at location 40 to a long integer
    secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
    secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
    secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
    secsSince1900 |= (unsigned long)packetBuffer[43];
    if (secsSince1900 == 0)
    {
      //Serial.println(F(" FAIL1"));
      return -999;
    }
    //Serial.println(highWord << 16 | lowWord);
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900)
    *t = secsSince1900; //initialize time with epoch
    Serial.println(F(" OK"));

  }
  else {
    udp.stop();
    //Serial.println(F(" FAIL2"));
    return -999;
  }
  return NTP_end_time - NTP_start_time;
}


void timeManager(bool forceupdate)
{
  static uint32_t NTPupdate = 0; //timestamp on when to update from the NTP server
  uint32_t temptime; //temporary timestamp to get data from servers
  uint32_t NTPtime; //NTP timestamp

  //run this code only if the WIFI is available:
  if (WiFi.status() == WL_CONNECTED) {

    if (millis() - NTPupdate > 3600000 || forceupdate) //update local time from NTP once every 60 minutes
    {
      uint8_t errorcounter = 0;
      int roundtripdelay;
      uint32_t timevalidation;
      //update the local time
      do
      {
        roundtripdelay = NTP_gettime(&NTPtime); //get NTP time
        delay(100);
        roundtripdelay += NTP_gettime(&temptime); //get another timestamp to validate
        if (NTPtime > 1456051511) //true if we got a valid timestamp
        {
          timevalidation = temptime - NTPtime; //should be zero or one if we got the same time twice
        }
        else timevalidation = 2; //time we got is invalid for sure.
        errorcounter++;
      } while ((roundtripdelay <= 0 || roundtripdelay > 2000) && timevalidation > 1 && errorcounter < 8);

      if (errorcounter < 8) {
        localTimeValid = true;
        setTime(NTPtime - 2208988800UL); //initialize the time with epoch timestamp
        syncRTC(); //check sync of local RTC               
      }
       NTPupdate = millis(); //update again in a few minutes
    }
  }
}

