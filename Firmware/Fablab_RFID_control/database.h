/*
   using extended database library, writing to a file in the SPIFFS file system and to the SD card

   if a user is found in the database of this machine, the machine is unlocked
*/


#define MAXUSERSINDB 2000
#define USERDB_TABLE_SIZE 102400 //100kb database, can hold over 2000 user entries  todo: calculate this from MAXUSERSINDB

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
  }
  else //update unsuccessful: open userdb file (previous database)
  {
    userDBInit(db_users);
  }
}

//delete the user database (used when resetting to defaults)
void userDBpurge(void)
{
  userDBerase(db_users);
}

