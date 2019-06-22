/*
   using extended database library, writing to a file in the SPIFFS file system and to the SD card
   if a user is found in the database of this machine, the machine is unlocked

*/

#define MAXUSERSINDB 2000
#define USERDB_TABLE_SIZE 102400 //100kb database, can hold over 2000 user entries  todo: calculate this from MAXUSERSINDB

void SDwriteLogfile(String entry); //function prototype

const char* db_users = "/db/users.db";
const char* db_users2 = "/db/users2.db"; //backup table, used during server upates to not corrupt the user database in case there is a problem during the update
fs::File userDBfile; //user database file resides in SPIFFS, need to add namespace

void userDBwriter (unsigned long address, const uint8_t* data, unsigned int datasize) {
  userDBfile.seek(address, fs::SeekSet);
  userDBfile.write(data, datasize);
  userDBfile.flush();
}

void userDBreader (unsigned long address, uint8_t* data, unsigned int datasize) {
  userDBfile.seek(address, fs::SeekSet);
  userDBfile.read(data, datasize);
}

EDB userdatabase(&userDBwriter, &userDBreader); //create the database


//debug:
void DBprintError(EDB_Status err)
{
  Serial.print(F("ERROR: "));
  switch (err)
  {
    case EDB_OUT_OF_RANGE:
      Serial.println(F("Recno out of range"));
      break;
    case EDB_TABLE_FULL:
      Serial.println(F("Table full"));
      break;
    case EDB_OK:
    default:
      Serial.println("OK");
      break;
  }
}


//open a database file, create it if it does not exist
void userDBInit(const char* databasepath)
{
  if (userDBfile) userDBfile.close(); //close the databasefile if it is open

  if (SPIFFS.exists(databasepath)) {

    userDBfile = SPIFFS.open(databasepath, "r+");

    if (userDBfile) {
      Serial.print(F("Opening current table... "));
      EDB_Status result = userdatabase.open(0);
      if (result == EDB_OK) {

        if (userdatabase.count() > MAXUSERSINDB)
        {
          Serial.println("error: too many users");
        }
        else
        {
          Serial.println("DONE");
          return;
        }
      }
      else {
        Serial.println(F("ERROR"));
        Serial.print(F("Did not find database in the file "));
        Serial.println(String(db_users));
        Serial.print(F("Creating new table... "));
        userdatabase.create(0, USERDB_TABLE_SIZE, (unsigned int)sizeof(userentry));
        Serial.println("DONE");
        return;
      }
    } else {
      Serial.println("Could not open file " + String(databasepath));
      //delete the corrupt file
      SPIFFS.remove(databasepath);
      //create and open new file now
    }
  }

  Serial.print(F("Creating user database... "));
  // create table at with starting address 0
  userDBfile = SPIFFS.open(databasepath, "w+");
  userdatabase.create(0, USERDB_TABLE_SIZE, (unsigned int)sizeof(userentry));
  Serial.println(F("DONE"));

}

//default initialization
void userDBInit(void)
{
  userDBInit(db_users);
}

void userDBclose(void)
{
  if (userDBfile)  //check if a file is open
  {
    userDBfile.close(); //and close it
  }
}

//erase a user database (closes any open database file just in case -> need to call init database after deleting one)
void userDBerase(const char* databasepath)
{
  if (SPIFFS.exists(databasepath)) {

    userDBclose(); //this is to prevent reading a non-existing file if the deleted file is currently open, there would be more elegant ways to do this but this is easiest and works fine
    SPIFFS.remove(databasepath);
  }
}

//search user database for a uid, return entry number if found, 0 if not found (takes 900ms at 500 entries)
uint16_t userDBfindentry(uint32_t RFIDuid)
{
  uint16_t i;
  Serial.print(F("number of user DB entries: "));
  Serial.println(userdatabase.count());
  for (i = 1; i <= userdatabase.count(); i++) //entries start from 1
  {
    EDB_Status result = userdatabase.readRec(i, EDB_REC userentry);
    if (result == EDB_OK)
    {
      if (userentry.uid == RFIDuid) //entry found
      {
        return i;
      }
    }
  }
  return 0;
}

/*
  void userDBdeleteentry(uint32_t RFIDuid) //obsolete
  {
  Serial.print(F("Deleting entry... "));
  //go through all database entries, delete if found
  uint16_t entryno = userDBfindentry(RFIDuid);
  if (entryno > 0) //if entry was found
  {
    userdatabase.deleteRec(entryno);
  }

  Serial.println(F("DONE"));
  }
*/
//add an etry to the secondary userDB
bool userDBaddentry(uint16_t tid, uint32_t RFuid, uint32_t validfrom, uint32_t validuntil, const char* username) //obsolete
{
  Serial.print(F("Appending userDB entry... "));

  userentry.tagid = tid;
  userentry.uid = RFuid;
  userentry.ts_validfrom = validfrom;
  userentry.ts_validuntil = validuntil;
  strncpy(userentry.name, username, 31);
  userentry.name[31] = '\0'; //securely NUL terminate the string (owner should never be larger than 30 but this makes it safer for buffer overflows)
  EDB_Status result = userdatabase.appendRec(EDB_REC userentry);
  if (result != EDB_OK) {
    DBprintError(result);
    return false;
  }
  //Serial.println(F("DONE"));
  return true;
}

//for debugging purposes
void userDBprintout(void)
{
  uint16_t i;
  Serial.print(F("number of user DB entries: "));
  Serial.println(userdatabase.count());
  for (i = 1; i <= userdatabase.count(); i++) //entries start from 1
  {
    delay(1); //run background functions
    EDB_Status result = userdatabase.readRec(i, EDB_REC userentry);
    Serial.print(F("TID: "));
    Serial.println(userentry.tagid);
    Serial.print(F("User: "));
    Serial.println(userentry.name);
    Serial.print(F("UID: "));
    Serial.println(userentry.uid);
    Serial.print(F("start: "));
    Serial.println(userentry.ts_validfrom);
    Serial.print(F("end: "));
    Serial.println(userentry.ts_validuntil);
  }
}

//prepare database for update: close current DB file, erase secondary DB (in case it already exists), create secondary DB file and open it

void prepareForDBupdate(void)
{
  userDBclose();
  userDBerase(db_users2);
  userDBInit(db_users2);
}

//cleanup after an update, argument is whether the update was deemed successful or not (true if successful)
void updateCleanup(bool successful)
{
  userDBclose(); //close current database file

  if (successful) //update was successful:  delete usserdb file, rename userdb2 to userdb
  {
    userDBerase(db_users);
    SPIFFS.rename(db_users2, db_users);
    userDBInit(db_users);
    SDwriteLogfile("UserDB updated");
  }
  else //update unsuccessful: open userdb file (previous database)
  {
    userDBInit(db_users);
    SDwriteLogfile("UserDB update failed");
  }
}

//delete the user database (used when resetting to defaults)
void userDBpurge(void)
{
  userDBerase(db_users);
}

//////////////////////////////////////////////
// EVENT DATABASE FOR SPIFFS /////////////////
//////////////////////////////////////////////

#define EVENTDB_TABLE_SIZE 8192 //event database table size (on SPIFFS or SD card) to store unsent events
int16_t eventDBentrytosend; //index of the event currently being transmitted from the event database (send one by one to have better database control)
sendoutpackage eventDBpackage; //buffer for one package to be read from DB and sent out

//function prototypes (todo: need to clean up the header file depedency mess sometime)
void sendToServer(sendoutpackage* datastruct, bool save, bool enforce);


#ifndef EVENT_DB_TO_SD //using SPIFFS for event database
//SD card database for storing unsent events
const char* db_events = "events.db"; //file for storing events database
fs::File eventDBfile; //event database resides in SPIFFS, need to add namespace

void eventDBwriter (unsigned long address, const uint8_t* data, unsigned int datasize) {
  eventDBfile.seek(address, fs::SeekSet);
  eventDBfile.write(data, datasize);
  eventDBfile.flush();
}

void eventDBreader (unsigned long address, uint8_t* data, unsigned int datasize) {
  eventDBfile.seek(address, fs::SeekSet);
  eventDBfile.read(data, datasize);
}

EDB eventdatabase(&eventDBwriter, &eventDBreader); //create the event database

//open a database file, create it if it does not exist
void eventDBInit(void)
{
  Serial.print(F("EventDB init: "));

  if (SPIFFS.exists(db_events)) {
    Serial.println(F("Event database file exists"));
    eventDBfile = SPIFFS.open(db_events, "r+"); //open file for reading and writing

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
      SPIFFS.remove(db_events);
      //now go on and create a new file with a new table
    }
  }
  Serial.print(F("Creating event database... "));
  // create table at with starting address 0
  eventDBfile = SPIFFS.open(db_events, "w+"); //create file, overwrite if it exists (w+)
  eventdatabase.create(0, EVENTDB_TABLE_SIZE, (unsigned int)sizeof(eventDBpackage));
  Serial.println(F("DONE"));
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

//add an entry to the eventDatabase
bool eventDBaddentry(sendoutpackage* evententry)
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


//read a single pending event from the database (if any) into the eventDBpackage (can only be called from within this file)
void eventDBgetpending(void)
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

void checkEventDB(void)
{
  static unsigned long EVDBchecktime = millis();
  static bool EVDBeventPending = false; //set to true if events were saved, sendout is then repeated faster

  if (millis() - EVDBchecktime > 10000 || EVDBeventPending) //check for pending events in the database
  {
    EVDBchecktime = millis();
    eventDBgetpending(); //read an entry from the event database (if any) and save it to eventDBpackage struct
    if (eventDBpackage.pending)
    {
      EVDBeventPending = true;
      sendToServer(&eventDBpackage, false, false); //send to server, do not save again
      if (eventDBpackage.pending == false) //if sent out successfully, delete this entry from the database (sendout sets pending = false)
      {
        eventDBdeleteentry(eventDBentrytosend);
      }
    }
    else
    {
      EVDBeventPending = false;
    }
  }
}


#endif
