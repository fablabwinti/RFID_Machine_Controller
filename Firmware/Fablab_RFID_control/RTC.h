/*
   Time keeping functions for the local RTC

   Note: RTC runs on UNIX time, timezone has to be added, so does day light saving time (done automatically in the 'getTimeString' function)
         the local time can never assumed to be valid, always use the RTC for any time functions. to get the local time, including timezone and DST offsets
         use the updatelocaltimefromRTC function first.

BUG: the rtc can fail to sync to NTP server... unknown why (date returned and date set are not equal, also the date is pi day so it seems RTCTimeValid == false)
Connected to:
SSID: IUY-12651
IP: 192.168.1.158
Checking pending events on DB
SD database file exists
Opening eventDB table... DONE
number of event DB entries: 19
read entry from EventDB
converting 0
Sendoutdata: 
{"timestamp":"2014-03-14 03:14:15","mid":3,"event":2,"remarks":"error Event Test at Bootup"}
Server connect failed
Updating user database...
connecting to 192.168.1.25:3000
connection failed
NTP Sync  OK
NTP Sync  OK
Synchronizing RTC with NTP... RTC set: 2009 85 165 37.165:85
2014-03-14T03:14:15.927Z  Connected to WIFI IP = 192.168.1.158


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

//read current time from RTC and return it as a UNIX timestamp
time_t getRtcTimestamp(void)
{
  if (RTCTimeValid)
  {
    RtcDateTime RTCtime = RTC.GetDateTime();
    //Serial.println(RTCtime.Epoch32Time());
    return RTCtime.Epoch32Time();
  }
  else return 0;
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
    createErrorEvent("RTC Error (battery empty?)");
  }
  Serial.print(F("Local time is "));
  String timenow = getTimeString();

  Serial.println(timenow); //updates local time from RTC

  //display.print(F("RTC Time: "));
  //display.println(timenow);
  //display.display();

}

//returns date and time string of current local time, "2014-03-14T03:14:15.927Z" if no time is available (pi day 2014, founding date of FablabWinti)
//get time string for logging and debug
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

    datetime = String(year()) + "-" + monthstr + "-" + daystr + "T" + hourstr + ":" + minutestr + ":" + secondstr + ".000Z";
  }
  else datetime = "2014-03-14T03:14:15.927Z";

  return datetime;
}

//convert a timestamp to a time string
String convertToTimesting(time_t timestamp)
{
  Serial.print("converting ");
  Serial.println(timestamp);
  String datetime;
  RtcDateTime RTCtime;
  if(timestamp < 1394766855) timestamp = 1394766855; //add a dummy time so server accepts it (pi day 2014)
  RTCtime.InitWithEpoch32Time(timestamp);
  char temparr[5];
  sprintf(temparr, "%02u", RTCtime.Month()); //need a fixed length, easiest using sprintf
  String monthstr = String(temparr);
  sprintf(temparr, "%02u", RTCtime.Day()); //need a fixed length, easiest using sprintf
  String daystr = String(temparr);
  sprintf(temparr, "%02u", RTCtime.Hour()); //need a fixed length, easiest using sprintf
  String hourstr = String(temparr);
  sprintf(temparr, "%02u", RTCtime.Minute()); //need a fixed length, easiest using sprintf
  String minutestr = String(temparr);
  sprintf(temparr, "%02u", RTCtime.Second()); //need a fixed length, easiest using sprintf
  String secondstr = String(temparr);

  datetime = String(RTCtime.Year()) + "-" + monthstr + "-" + daystr + " " + hourstr + ":" + minutestr + ":" + secondstr;

//format: "2017-06-08 22:24:15"
  return datetime;
}


/*
   to parse a datestring into local time format, the time library can be used, but first, the string has to be parsed into integers, something like this:
   int commaIndex = myString.indexOf(',');
  //  Search for the next comma just after the first
  int secondCommaIndex = myString.indexOf(',', commaIndex + 1);
  Then you could use that index to create a substring using the String class's substring() method. This returns a new String beginning at a particular starting index, and ending just before a second index (Or the end of a file if none is given). So you would type something akin to:

  String firstValue = myString.substring(0, commaIndex);
  String secondValue = myString.substring(commaIndex + 1, secondCommaIndex);
  String thirdValue = myString.substring(secondCommaIndex + 1); // To the end of the string


*/
