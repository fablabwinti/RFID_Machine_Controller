/*
  note spiffs & SDfat:
  using namespacesdfat funktioniert.
  vor dem spiffs file gehöert ein fs:: sonst nix.
  #define FS_NO_GLOBALS //allow spiffs to coexist with SD card
  #include <FS.h> //spiff file system
  using namespace sdfat;
  SdFat SD;
  File dbFile; //ist ein SD file
  fs::File spifffile; //ist ein spiffs file

  Some code used here was taken from the time and SD arduino libraries. See

  https://github.com/PaulStoffregen/Time  (licensed under GPL)
  https://github.com/arduino/Arduino/tree/master/libraries/SD (licensed under LGPL)

  Note on formatting SD cards: the arduino website recommends FAT16 even though FAT32 is also supported

  The SD card holds a human readable system log file and a database to store unsent events. The database can hold
  100kB of data (can be increased if necessary) entries are deleted once they are sent to the server so it acts
  just as a persistant buffer. If events have to be logged permanently, multiple files should be created (not implemented)

*/

#define LEAP_YEAR(Y)     ( ((1970+Y)>0) && !((1970+Y)%4) && ( ((1970+Y)%100) || !((1970+Y)%400) ) )  //from time library

#define SD_UNKNOWN 0
#define SD_PRESENT 1
#define SD_NOTPRESENT 2
#define SD_INITIALIZED 3
#define SD_REMOVED 4
#define SD_ACESSFAILED 5


SdFat SD; //using SdFat instead of SD library, much faster and more stable


uint8_t SDstate = SD_UNKNOWN;

#ifdef EVENT_DB_TO_SD
//SD card database for storing unsent events
const char* db_events = "events.db"; //file for storing events database
File eventDBfile; //event database resides on the SD card (it may be written often, if worn out, the SD card is easy to replace campared to SPIFFs flash on the ESP8266 board)

void eventDBwriter (unsigned long address, const uint8_t* data, unsigned int datasize) {
  eventDBfile.seek(address);
  eventDBfile.write(data, datasize);
  eventDBfile.flush();
}

void eventDBreader (unsigned long address, uint8_t* data, unsigned int datasize) {
  eventDBfile.seek(address);
  eventDBfile.read(data, datasize);
}

EDB eventdatabase(&eventDBwriter, &eventDBreader); //create the event database

//open a database file, create it if it does not exist
void eventDBInit(void)
{
  Serial.print(F("EventDB init: "));
  if (SDstate == SD_INITIALIZED)
  {
    if (SD.exists(db_events)) {
      Serial.println(F("SD database file exists")); //!!!
      eventDBfile = SD.open(db_events, FILE_WRITE); //open file for reading and writing

      if (!eventDBfile) //if open fails, try once more
      {
        eventDBfile = SD.open(db_events, FILE_WRITE); //open file for reading and writing
      }

      if (eventDBfile) {
        Serial.print(F("Opening eventDB table... "));
        EDB_Status result = eventdatabase.open(0);
        if (result == EDB_OK) {
          Serial.println("DONE");
          return;
        } else {
          Serial.println(F("ERROR"));
          Serial.print(F("Did not find eventDB in the file "));
          Serial.println(String(db_events));
          Serial.print(F("Creating new table... "));
          eventdatabase.create(0, EVENTDB_TABLE_SIZE, (unsigned int)sizeof(eventDBpackage));
          Serial.println("DONE");
          return;
        }
      } else {
        Serial.println("Could not open file " + String(db_events));
        //delete the corrupt file
        SD.remove(db_events);
        //now go on and create a new file with a new table
      }
    }
    Serial.print(F("Creating event database... "));
    // create table at with starting address 0
    eventDBfile = SD.open(db_events, FILE_WRITE); //create file, overwrite if it exists (w+)
    eventdatabase.create(0, EVENTDB_TABLE_SIZE, (unsigned int)sizeof(eventDBpackage));
    Serial.println(F("DONE"));
  }
}


void eventDBclose(void)
{
  if (eventDBfile)  //check if a file is open
  {
    eventDBfile.close(); //and close it
  }
}


void eventDBdeleteentry(uint16_t entryno)
{
  if (SDstate == SD_INITIALIZED)
  {
    if (!eventDBfile) //database file not open, so open the database
    {
      eventDBInit();
    }

    Serial.print(F("Deleting entry... "));
    if (entryno <= eventdatabase.count()) //if entry exists (entries are from 1 to count, not starting at 0!)
    {
      EDB_Status result = eventdatabase.deleteRec(entryno);
      if (result != EDB_OK) DBprintError(result);
      Serial.println("DONE");
    }
    else
    {
      Serial.println(F("NOT FOUND"));
    }
    eventDBclose();
  }

}

//add an entry to the eventDatabase
bool eventDBaddentry(sendoutpackage* evententry)
{
  if (SDstate == SD_INITIALIZED)
  {
    if (!eventDBfile) //database file not open, so open the database
    {
      eventDBInit();
    }


    Serial.print(F("Appending eventDB entry... "));
    ESP.wdtFeed(); //kick hardware watchdog

    //make sure the package is pending:
    evententry->pending = true;
    EDB_Status result = eventdatabase.appendRec(EDB_REC * evententry); //eventDBpackage is passed as a pointer but is dereferenced first, then cast back into a pointer by EDB_REC macro (kind of convoluted... see library definitions for details)
    if (result != EDB_OK) {
      DBprintError(result);
      eventDBclose();
      return false;
    }
    Serial.println(F("DONE"));
    eventDBclose();
    //entry is now in the database, remove it from the queue
    evententry->pending = false;
    return true;
  }
  Serial.print(F("Error saving to SD card, event not saved!"));
}


//read a single pending event from the database (if any) into the eventDBpackage (can only be called from within this file)
void eventDBgetpending(void)
{
  if (SDstate == SD_INITIALIZED)
  {
    //Serial.println(F("Checking pending events on DB"));
    if (!eventDBfile) //database file not open, so open the database
    {
      eventDBInit();
    }
    eventDBpackage.pending = false; //reset pending (use it to check in calling function to check if data was even read)
    uint16_t i;
    //Serial.print(F("number of event DB entries: "));
    //Serial.println(eventdatabase.count());
    for (i = eventdatabase.count(); i > 0; i--) //start scanning from the end (deleting end entries is faster), also, zero index is not used in EDB
    {
      EDB_Status result = eventdatabase.readRec(i, EDB_REC eventDBpackage); //eventDBpackage is passed as a pointer
      if (result == EDB_OK)
      {
        Serial.println(F("read entry from EventDB"));
        eventDBentrytosend = i;
        break; //end the for loop now
      }
    }
    eventDBclose();
  }
}

#endif


//human readable logfile stuff

void SDwriteLogfile(String entry)
{
  Serial.println(F("writing to log"));
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
    ESP.wdtFeed(); //kick hardware watchdog
    if (SD.exists(filename) == false) //file does not exist, write a header
    {
      File dataFile = SD.open(filename, FILE_WRITE); //create the file, open for writing, start at the end of the file
      if (dataFile) {
        dataFile.println(F("RFID Controller Logfile"));
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


void SDwriteStringToLog(String data) //writes a string to SD card, one data log file per day
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
        String header = "RFID backup log " + getTimeString();
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
      dataFile.println(data); //since this is used to write the JSON string which is sent also to the server, it contains the timestamp already
      dataFile.close();
      yield();
      Serial.println(F("data written to SD log"));
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


//from SD library example:
void printDirectory(sdfat::File dir, int numTabs) {
  if (SDstate == SD_INITIALIZED)
  {
    while (true) {
      sdfat::File entry =  dir.openNextFile();
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
  ESP.wdtFeed(); //kick hardware watchdog
  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH);
  Serial.print(F("SD init... "));
  display.print(F("SD init... "));
  display.display();
  //check SD detect pin, sd is present if it is low
  if (analogRead(A0) < 800)
  {
    SDstate = SD_PRESENT;
    if (!SD.begin(pin)) {
      yield();
      Serial.println(F("card failed"));
      display.println(F("SD card failed"));
      display.display();
      createErrorEvent("SD card read error");
      return false;
    }
    else
    {
      SDstate = SD_INITIALIZED;
      Serial.println(F("SD card initialized."));
      display.println(F("OK"));
      display.display();
      SDcardOK = true;
      /*
        sdfat::File dir = SD.open("/");
        printDirectory(dir, 0);
        dir.close();
      */
      return true;
    }
  }
  else
  {
    Serial.println(F("Card not present"));
    display.println(F("no card"));
    display.display();
    SDstate = SD_NOTPRESENT;
    createErrorEvent("SD card not present");
    return false;
  }
}

//SD card manager function, call this repeatedly in the loop, detects when SD card is removed, sends pending events from SD event-database
void SDmanager(void)
{
  static unsigned long SDchecktime = millis();
  static bool SDeventPending = false; //set to true if events were saved to SD, sendout is then repeated faster
  if (SDstate == SD_UNKNOWN) //first time run
  {
    SDinit(SD_CSN_PIN);
  }
  else
  {
    if (millis() - SDchecktime > 1000 || SDeventPending) //check the state of the SD card once every second, this function also checks for pending events in the SD database
    {
      SDchecktime = millis();
      //check the SD state:
      if (SDstate == SD_INITIALIZED)
      {
        if (analogRead(A0) > 900) //card is not present now, means it was removed
        {
          SDstate = SD_REMOVED;
          Serial.println(F("SDcard Removed"));
          createErrorEvent("SD card removed");
          SDcardOK = false;
          SDeventPending = false;
        }
#ifdef EVENT_DB_TO_SD
        else //SD is initialized and ready: check for pending events (if using SD for events database)
        {
          eventDBgetpending();
          if (eventDBpackage.pending)
          {
            SDeventPending = true;
            sendToServer(&eventDBpackage, false, false); //send to server, do not save again
            if (eventDBpackage.pending == false) //if sent out successfully, delete this entry from the database (sendout sets pending = false)
            {
              eventDBdeleteentry(eventDBentrytosend);
            }
          }
          else
          {
            SDeventPending = false;
          }
        }
#endif
        return;
      }
      else if (SDstate == SD_REMOVED)
      {
        if (analogRead(A0) < 800) //card is now present but was removed before -> reboot to reinitialize (cannot just reinitialize, not supported by library todo: check if this bug still exists)
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
        createErrorEvent("SD card error");
        SDcardOK = false;
        return;
      }
    }
  }
}
