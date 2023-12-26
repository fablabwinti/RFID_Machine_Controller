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
#ifdef SERIALDEBUG
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
#endif
}


//open a database file, create it if it does not exist
void userDBInit(const char* databasepath)
{
  if (userDBfile) userDBfile.close(); //close the databasefile if it is open

  if (SPIFFS.exists(databasepath)) {

    userDBfile = SPIFFS.open(databasepath, "r+");

    if (userDBfile) {
#ifdef SERIALDEBUG
      Serial.print(F("Opening current table... "));
#endif
      EDB_Status result = userdatabase.open(0);
      if (result == EDB_OK) {

        if (userdatabase.count() > MAXUSERSINDB)
        {
#ifdef SERIALDEBUG
          Serial.println("error: too many users");
#endif
        }
        else
        {
#ifdef SERIALDEBUG
          Serial.println("DONE");
#endif
          return;
        }
      }
      else {
#ifdef SERIALDEBUG
        Serial.println(F("ERROR"));
        Serial.print(F("Did not find database in the file "));
        Serial.println(String(db_users));
        Serial.print(F("Creating new table... "));
#endif
        userdatabase.create(0, USERDB_TABLE_SIZE, (unsigned int)sizeof(userentry));
#ifdef SERIALDEBUG
        Serial.println("DONE");
#endif
        return;
      }
    } else {
#ifdef SERIALDEBUG
      Serial.println("Could not open file " + String(databasepath));
#endif
      //delete the corrupt file
      SPIFFS.remove(databasepath);
      //create and open new file now
    }
  }
#ifdef SERIALDEBUG
  Serial.print(F("Creating user database... "));
#endif
  // create table at with starting address 0
  userDBfile = SPIFFS.open(databasepath, "w+");
  userdatabase.create(0, USERDB_TABLE_SIZE, (unsigned int)sizeof(userentry));
#ifdef SERIALDEBUG
  Serial.println(F("DONE"));
#endif

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
#ifdef SERIALDEBUG
  Serial.print(F("number of user DB entries: "));
  Serial.println(userdatabase.count());
#endif
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
#ifdef SERIALDEBUG
  Serial.print(F("Appending userDB entry... "));
#endif

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
  return true;
}

//for debugging purposes
void userDBprintout(void)
{
#ifdef SERIALDEBUG
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
#endif
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
#ifdef SERIALDEBUG
  Serial.print(F("EventDB init: "));
#endif
  if (SPIFFS.exists(db_events)) {
#ifdef SERIALDEBUG
    Serial.println(F("Event database file exists"));
#endif
    eventDBfile = SPIFFS.open(db_events, "r+"); //open file for reading and writing

    if (eventDBfile) {
#ifdef SERIALDEBUG
      Serial.print(F("Opening eventDB table... "));
#endif
      EDB_Status result = eventdatabase.open(0);
      if (result == EDB_OK) {
#ifdef SERIALDEBUG
        Serial.println("DONE");
#endif
        return;
      } else {
#ifdef SERIALDEBUG
        Serial.println(F("ERROR"));
        Serial.print(F("Did not find eventDB in the file "));
        Serial.println(String(db_events));
        Serial.print(F("Creating new table... "));
#endif
        eventdatabase.create(0, EVENTDB_TABLE_SIZE, (unsigned int)sizeof(eventDBpackage));
#ifdef SERIALDEBUG
        Serial.println("DONE");
#endif
        return;
      }
    } else {
#ifdef SERIALDEBUG
      Serial.println("Could not open file " + String(db_events));
#endif
      //delete the corrupt file
      SPIFFS.remove(db_events);
      //now go on and create a new file with a new table
    }
  }
#ifdef SERIALDEBUG
  Serial.print(F("Creating event database... "));
#endif
  // create table at with starting address 0
  eventDBfile = SPIFFS.open(db_events, "w+"); //create file, overwrite if it exists (w+)
  eventdatabase.create(0, EVENTDB_TABLE_SIZE, (unsigned int)sizeof(eventDBpackage));
#ifdef SERIALDEBUG
  Serial.println(F("DONE"));
#endif
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
#ifdef SERIALDEBUG
  Serial.print(F("Deleting entry... "));
#endif
  if (entryno <= eventdatabase.count()) //if entry exists (entries are from 1 to count, not starting at 0!)
  {
    // unless it's the last one, just mark it as unused, don't delete it, as that would
    // copy around the whole rest of the records using many separate write operations
    EDB_Status result;
    if (entryno == eventdatabase.count())
    {
      result = eventdatabase.deleteRec(entryno);
    }
    else
    {
      eventDBpackage.pending = false;
      result = eventdatabase.updateRec(entryno, EDB_REC eventDBpackage);
    }
    if (result != EDB_OK) DBprintError(result);
#ifdef SERIALDEBUG
    Serial.println("DONE");
#endif
  }
  else
  {
#ifdef SERIALDEBUG
    Serial.println(F("NOT FOUND"));
#endif
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
#ifdef SERIALDEBUG
  Serial.print(F("Appending eventDB entry... "));
#endif
  ESP.wdtFeed(); //kick hardware watchdog

  //make sure the package is pending:
  evententry->pending = true;
  EDB_Status result = eventdatabase.appendRec(EDB_REC * evententry); //eventDBpackage is passed as a pointer but is dereferenced first, then cast back into a pointer by EDB_REC macro (kind of convoluted... see library definitions for details)
  if (result != EDB_OK) {
    DBprintError(result);
    eventDBclose();
    return false;
  }
#ifdef SERIALDEBUG
  Serial.println(F("DONE"));
#endif
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
  for (i = 1; i <= eventdatabase.count(); i++) // start scanning from the beginning to keep chronological order, also, zero index is not used in EDB
  {
    EDB_Status result = eventdatabase.readRec(i, EDB_REC eventDBpackage); //eventDBpackage is passed as a pointer
    if (result == EDB_OK && eventDBpackage.pending)
    {
#ifdef SERIALDEBUG
      Serial.println(F("read entry from EventDB"));
#endif
      eventDBentrytosend = i;
      break; //end the for loop now
    }
  }
  if (!eventDBpackage.pending) {
    // whole table is unused, can truncate it
    eventdatabase.clear();
  }
  eventDBclose();

}


#endif
