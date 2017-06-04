/*
 * Time keeping functions for the local RTC
 * 
 */

RtcDS3231 RTC; //DS3231 RTC clock on I2C

void updatelocaltimefromRTC(void)
{
  Serial.print(F("Updating from RTC..."));
  RtcDateTime RTCtime  = RTC.GetDateTime();
  RtcDateTime RTCtemp  = RTC.GetDateTime();
  if (RTCtime.Epoch32Time() > 1496578319) //check if RTC could be valid
  {
    setTime(RTCtime.Epoch32Time()); //initialize the time with epoch timestamp
    localTimeValid = true;
  }
  else
  {
    RTCTimeValid = false;
    Serial.println(F("sailed"));
    return;
  }
  Serial.println(F("success"));
}

//sync the RTC to the local time (if valid)
void syncRTC(void)
{
  Serial.print(F("Synchronizing RTC with NTP... "));

  RtcDateTime RTCtime = RTC.GetDateTime();
  if (localTimeValid)
  {
    //check if RTC is out of sync by more than 2 seconds, sync only if it is (not sure if writing RTC all the time will wear down its memory, probably not)
    int8_t timedifference = now() - RTCtime.Epoch32Time();
    if (timedifference > 2 || timedifference < -2)
    {
      RTCtime.InitWithEpoch32Time(now());
      RTC.SetDateTime(RTCtime);
      Serial.print(F("RTC set: "));
      Serial.print(RTCtime.Year());
      Serial.print(" ");
      Serial.print(RTCtime.Month());
      Serial.print(" ");
      Serial.print(RTCtime.Day());
      Serial.print(" ");
      Serial.print(RTCtime.Hour());
      Serial.print(".");
      Serial.print(RTCtime.Minute());
      Serial.print(":");
      Serial.println(RTCtime.Second());

      //read back the time from the RTC to check if it is valid
      RTCtime  = RTC.GetDateTime();

      if (RTCtime.Epoch32Time() == now()) //check if RTC was set properly
      {
        RTCTimeValid = true;
      }
      else
      {
        RTCTimeValid = false;
      }
    }
    else
    {
        RTCTimeValid = true;
    }
  }

  if(RTCTimeValid) Serial.println(F("OK"));
  

}


void RTCinit(void)
{
  Serial.print(F("RTC init..."));
  RTC.Begin();
  RTC.Enable32kHzPin(false);
  RTC.SetSquareWavePin(DS3231SquareWavePin_ModeNone);
  RTC.SetIsRunning(true); //start the RTC in case it was not running (prevents infinite loop)
  RtcDateTime RTCtime  = RTC.GetDateTime();

  if (RTCtime.Epoch32Time() > 1496578319 && RTC.GetIsRunning()) //check if RTC could be valid
  {
    RTCTimeValid = true;
    updatelocaltimefromRTC();
    Serial.println(F("OK"));
  }
  else
  {
    Serial.println(F("RTC Failed"));
    RTCTimeValid = false;
  }
}


