/*
   Time keeping functions for the local RTC

   Note: RTC runs on UNIX time, timezone has to be added, so does day light saving time (done automatically in the 'getTimeString' function)
         the local time can never assumed to be valid, always use the RTC for any time functions. to get the local time, including timezone and DST offsets
         use the updatelocaltimefromRTC function first.



*/

RtcDS3231 RTC; //DS3231 RTC clock on I2C
String getTimeString(); //function prototype


//check if the input is in european summer or winter time (daylight saving time checker)
boolean isSummertime(tmElements_t &tm)
{
  if (tm.Month < 3 || tm.Month > 10) return false; // keine Sommerzeit in Jan, Feb, Nov, Dez
  if (tm.Month > 3 && tm.Month < 10) return true; // Sommerzeit in Apr, Mai, Jun, Jul, Aug, Sep

  //Umstellung auf Sommerzeit am letzten Sonntag im März (Wday ist Wochentag, sonntag ist 1)
  if (tm.Month == 3)
  {
    if ((tm.Day - (tm.Wday - 1) >= 25) && tm.Hour > 1) return true;
    else return false;
  }
  else if (tm.Month == 10)
  {
    if ((tm.Day - (tm.Wday - 1) >= 25) && tm.Hour > 2) return false;
    else return true;
  }

  return false; //line should never be reached
}

//update the local time, add timezone and DST to the RTC time
void updatelocaltimefromRTC(void)
{
  //Serial.print(F("Updating from RTC..."));
  RtcDateTime RTCtime  = RTC.GetDateTime();
  localTimeValid = false;
  if (RTCtime.Epoch32Time() > 1496578319) //check if RTC could be valid
  {
    time_t localtimestamp = RTCtime.Epoch32Time() + (TIMEZONE) * 3600 ;
    tmElements_t localTimeElements;
    breakTime(localtimestamp, localTimeElements); //break the Time into year, month, day, etc.
    if (isSummertime(localTimeElements)) //check if the time in the timezone is summer or winter time
    {
      localtimestamp += 3600; //add an hour if summer time
    }

    setTime(localtimestamp); //set the local time
    localTimeValid = true;
  }
  else
  {
    RTCTimeValid = false;
    return;
  }
  //Serial.println(F("success"));


}

//sync the RTC to the given time (UNIX time)
void syncRTC(uint32_t epochtime)
{
  RtcDateTime RTCtime = RTC.GetDateTime();

  //check if RTC is out of sync by more than 2 seconds, sync only if it is (not sure if writing RTC all the time will wear down its memory, probably not)
  int32_t timedifference = epochtime - RTCtime.Epoch32Time();
  if (timedifference > 2 || timedifference < -2)
  {
    RTCtime.InitWithEpoch32Time(epochtime);
    RTC.SetDateTime(RTCtime);
    //read back the time from the RTC to check if it is valid
    RTCtime  = RTC.GetDateTime();

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

    if (RTCtime.Epoch32Time() == epochtime) //check if RTC was set properly
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


  if (RTCTimeValid) Serial.println(F("OK"));
  updatelocaltimefromRTC(); //sync local time to RTC

}

time_t getRtcTimestamp(void)
{
  RtcDateTime RTCtime = RTC.GetDateTime();
  //Serial.println(RTCtime.Epoch32Time());
  return RTCtime.Epoch32Time();
}


void RTCinit(void)
{
  Serial.print(F("RTC init..."));


  display.print(F("RTC init..."));
  display.display();

  RTC.Begin();
  RTC.Enable32kHzPin(false);
  RTC.SetSquareWavePin(DS3231SquareWavePin_ModeNone);
  RTC.SetIsRunning(true); //start the RTC in case it was not running (prevents infinite loop)
  RtcDateTime RTCtime  = RTC.GetDateTime();

  if (RTCtime.Epoch32Time() > 1496578319 && RTC.GetIsRunning()) //check if RTC could be valid
  {
    RTCTimeValid = true;

    Serial.println(F("OK"));

    display.println(F("OK"));
    display.display();
  }
  else
  {
    Serial.println(F("Failed"));
    display.println(F("Failed"));
    display.display();
    RTCTimeValid = false;
  }
  Serial.print(F("Local time is "));
  String timenow = getTimeString();

  Serial.println(timenow); //updates local time from RTC

  //display.print(F("RTC Time: "));
  //display.println(timenow);
  //display.display();

}

//returns date and time string of current local time, "X" if no time is available
String getTimeString()
{
  String datetime;
  if (RTCTimeValid)
  {
    //set the local time from the RTC, add timezone and DST offset
    updatelocaltimefromRTC();

    char temparr[5];
    sprintf(temparr, "%02u", month()); //need a fixed length, easiest using sprintf
    String monthstr = String(temparr);
    sprintf(temparr, "%02u", day()); //need a fixed length, easiest using sprintf
    String daystr = String(temparr);
    sprintf(temparr, "%02u", hour()); //need a fixed length, easiest using sprintf
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
