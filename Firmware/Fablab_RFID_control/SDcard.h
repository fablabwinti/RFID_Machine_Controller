/*


  Some code used here was taken from the time and SD arduino libraries. See

  https://github.com/PaulStoffregen/Time  (licensed under GPL)
  https://github.com/arduino/Arduino/tree/master/libraries/SD (licensed under LGPL)

*/

#define LEAP_YEAR(Y)     ( ((1970+Y)>0) && !((1970+Y)%4) && ( ((1970+Y)%100) || !((1970+Y)%400) ) )  //from time library

#define SD_UNKNOWN 0
#define SD_PRESENT 1
#define SD_NOTPRESENT 2
#define SD_INITIALIZED 3
#define SD_REMOVED 4
#define SD_ACESSFAILED 5

uint8_t SDstate = SD_UNKNOWN;

void SDwriteLogfile(String entry)
{
  if (SDstate == SD_INITIALIZED)
  {

    String datatoWrite;
    String sfilename = "logfile.txt";

    datatoWrite = getTimeString(); //get a string of current time. function returns 'X' if time not available
    datatoWrite += "\t";

    datatoWrite += entry; //add the log entry to the file
    char filename[sizeof(sfilename)];
    sfilename.toCharArray(filename, sizeof(filename));
    yield(); //writing data could take long, better yield before opening file and writing data

    if (SD.exists(filename) == false) //file does not exist, write a header
    {
      File dataFile = SD.open(filename, FILE_WRITE); //create the file
      if (dataFile) {
        dataFile.println("Alpha Node Logfile");
      }
      dataFile.close();
    }

    // open the file
    File dataFile = SD.open(filename, FILE_WRITE);

    if (dataFile) {
      dataFile.println(datatoWrite);
      dataFile.close();
      yield();
      Serial.println("Logfile Written");
    }
    else
    {
      yield();
      SDstate = SD_ACESSFAILED;
      dataFile.close();
    }
  }
}


void SDwriteNodeDataFileEntry(String data) //writes a string to SD card, one data log file per day
{
  //the time is logged in UNIX time, seconds since Jan 1. 1970
  //time is read from NTP-server, 'X' is written if no real time is available
  if (SDstate == SD_INITIALIZED)
  {
    uint32_t UNIXtime = now();

    //caclulate the year (calculation from time library)
    uint16_t year = 0;
    uint32_t day =  UNIXtime / 86400UL; //full days since Jan 1. 1970
    uint32_t tempdays = 0; //count the days in full years
    while ((unsigned)(tempdays += (LEAP_YEAR(year) ? 366 : 365)) <= day) {
      year++;
    }
    //now we have the full years since 1970 and the number of days in those ears stored in tempday.
    //deduct the days added in the last loop from tempday and remove the remaining days from the days since 1970
    //and we get the days in this year starting from 0, so add 1 to start at day 1

    tempdays -= LEAP_YEAR(year) ? 366 : 365;
    year += 1970;
    day  -= tempdays;
    day++; //add one day to start at day 1 in a year

    char daystring[5];
    sprintf(daystring, "%03u", day); //fixed length number (002, 073, etc)

    String sfilename = String(year) + String(daystring) + ".txt";
    // Serial.print("Filename: ");
    // Serial.println(sfilename);


    char filename[sizeof(sfilename)];
    sfilename.toCharArray(filename, sizeof(filename));

    if (SD.exists(filename) == false) //file does not exist, create it abd write a header
    {
      File dataFile = SD.open(filename, FILE_WRITE); //create the file
      if (dataFile) {
        String header = "Lambda Nodes data " + getTimeString();
        dataFile.println(header);
      }
      dataFile.close();
    }
    yield();
    // open the file
    File dataFile = SD.open(filename, FILE_WRITE);

    // if the file is available, write to it:
    if (dataFile) {
      //String datatoWrite;
      //datatoWrite = getTimeString(); //get a string of current time. function returns 'X' if time not available
      //datatoWrite += "\t";
      //datatoWrite += data; //data string (in JSON format)
      dataFile.println(data);
      dataFile.close();
      yield();
      Serial.println(F("data written to SD"));
    }
    // if the file cannot be opened:
    else {
      Serial.println(F("error opening SD file"));
      yield();
      SDstate = SD_ACESSFAILED;
      dataFile.close();
    }
  }

}


void SDwriteNodeData(uint8_t index)
{
  if (SDstate == SD_INITIALIZED)
  {
    String datastr = "{";
    datastr += nodeDatatosend[index].timestamp;
    datastr += ",";
    datastr += nodeDatatosend[index].ID;
    datastr += ",";
    datastr += nodeDatatosend[index].logstring;
    datastr += "}";
    SDwriteNodeDataFileEntry(datastr);
  }
}

//from SD library example:
void printDirectory(File dir, int numTabs) {
  if (SDstate == SD_INITIALIZED)
  {
    while (true) {
      File entry =  dir.openNextFile();
      if (! entry) {
        // no more files
        break;
      }
      for (uint8_t i = 0; i < numTabs; i++) {
        Serial.print('\t');
      }
      Serial.print(entry.name());
      if (entry.isDirectory()) {
        Serial.println("/");
        printDirectory(entry, numTabs + 1);
      } else {
        // files have sizes, directories do not
        Serial.print("\t\t");
        Serial.println(entry.size(), DEC);
      }
      entry.close();
    }
  }
}


bool SDinit(uint8_t pin)
{
  yield();

  pinMode(SD_CSN_PIN, OUTPUT);
  digitalWrite(SD_CSN_PIN, HIGH);
  Serial.print(F("SD init... "));
  //check SD detect pin, sd is present if it is low
  if (analogRead(A0) < 800)
  {
    SDstate = SD_PRESENT;
    if (!SD.begin(pin)) {
      yield();
      Serial.println(F("Card failed"));
      //todo: add an LED feedback here
      return false;
    }
    else
    {
      SDstate = SD_INITIALIZED;
      Serial.println(F("SD card initialized."));
      File dir = SD.open("/");
      printDirectory(dir, 0);
      dir.close();
      return true;
    }
  }
  else
  {
    Serial.println(F("Card not present"));
    SDstate = SD_NOTPRESENT;
    return false;
  }
}

//SD card manager function, call this repeatedly in the loop, detects when SD card is removed
void SDmanager(void)
{
  static unsigned long SDchecktime = millis();

  if (SDstate == SD_UNKNOWN) //first time run
  {
    SDinit(SD_CSN_PIN);
  }
  else
  {
    if (millis() - SDchecktime > 1000)
    {
      SDchecktime = millis();
      //check the SD state:
      if (SDstate == SD_INITIALIZED)
      {
        if (analogRead(A0) > 900) //card is not present now, means it was removed
        {
          SDstate = SD_REMOVED;
          Serial.println(F("SDcard Removed"));
        }
        return;
      }
      else if (SDstate == SD_REMOVED)
      {
        if (analogRead(A0) < 800) //card is now present but was removed before -> reboot to reinitialize (cannot just reinitialize, not supported by library)
        {
          Serial.println(F("SD detected, rebooting..."));
          ESP.restart();
        }
      }
      else if (SDstate == SD_NOTPRESENT)
      {
        if (analogRead(A0) < 800) //card is now present
        {
          Serial.println(F("SD detected"));
          SDinit(SD_CSN_PIN);
        }
        return;
      }
      else if (SDstate == SD_ACESSFAILED)
      {
        //todo: send a note to the server
        return;
      }
    }
  }
}

