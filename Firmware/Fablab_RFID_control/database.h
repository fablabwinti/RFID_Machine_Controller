/*
   using extended database library, writing to a file in the SPIFFS file system and to the SD card

   if a user is found in the database of this machine, the machine is unlocked
*/

#include <EDB.h>

#define USERDB_TABLE_SIZE 14400 //14kb database, can hold 400 user entries

//struct for user authentication

struct userAuth {
  uint32_t uid; //4 byte uid of the RFID card
  char firstname[16]; //16byte first name of user
  char familyname[16]; //16byte family name of user
} userentry;


char* db_users = "/db/users.db";
fs::File userDBfile; //user database file resides in SPIFFS, need to add namespace

void userDBwriter (unsigned long address, byte data) {
  userDBfile.seek(address, fs::SeekSet);
  userDBfile.write(data);
  userDBfile.flush();
}

byte userDBreader (unsigned long address) {
  userDBfile.seek(address, fs::SeekSet);
  return userDBfile.read();
}

EDB userdatabase(&userDBwriter, &userDBreader); //create the database


//debug:
void printError(EDB_Status err)
{
    Serial.print("ERROR: ");
    switch (err)
    {
        case EDB_OUT_OF_RANGE:
            Serial.println("Recno out of range");
            break;
        case EDB_TABLE_FULL:
            Serial.println("Table full");
            break;
        case EDB_OK:
        default:
            Serial.println("OK");
            break;
    }
}




void databaseInit(void)
{
  if (SPIFFS.exists(db_users)) {

    userDBfile = SPIFFS.open(db_users, "r+");

    if (userDBfile) {
      Serial.print("Opening current table... ");
      EDB_Status result = userdatabase.open(0);
      if (result == EDB_OK) {
        Serial.println("DONE");
      } else {
        Serial.println("ERROR");
        Serial.println("Did not find database in the file " + String(db_users));
        Serial.print("Creating new table... ");
        userdatabase.create(0, USERDB_TABLE_SIZE, (unsigned int)sizeof(userentry));
        Serial.println("DONE");
        return;
      }
    } else {
      Serial.println("Could not open file " + String(db_users));
      return;
    }
  } else {
    Serial.print("Creating table... ");
    // create table at with starting address 0
    userDBfile = SPIFFS.open(db_users, "w+");
    userdatabase.create(0, USERDB_TABLE_SIZE, (unsigned int)sizeof(userentry));
    Serial.println("DONE");
  }

}

//search user database for a uid, return entry number if found, 0 if not found
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

void userDBdeleteentry(uint32_t RFIDuid)
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

//add an etry to the userDB, update it if it exists and is different
void userDBaddentry(uint32_t RFIDuid, char* famname, char* frstname)
{
  Serial.print(F("Appending userDB entry... "));
  uint16_t entryno = userDBfindentry(RFIDuid); //check if entry already exists (we want no double entries), function reads the entry if it exists
  if (entryno > 0) //if it already exists, check if the name matches, update current entry if it does not
  {
    Serial.print(F("entry exists already "));
    uint8_t equal = memcmp(userentry.familyname , famname, 16) ;
    equal += memcmp(userentry.firstname , frstname, 16);
    if (equal == 0) //entry is identical
    {
      Serial.println(F(" and is identical"));
    }
    else
    {
      Serial.print(F(" and is different. updating... "));
      EDB_Status result = userdatabase.updateRec(entryno, EDB_REC userentry);
      if (result != EDB_OK) printError(result);
      Serial.println(F("DONE"));
    }
  }
  else
  {
    userentry.uid = RFIDuid;
    memcpy(userentry.familyname , famname, 16);
    memcpy(userentry.firstname , frstname, 16);
    EDB_Status result = userdatabase.appendRec(EDB_REC userentry);
    if (result != EDB_OK) printError(result);
    Serial.println(F("DONE"));
  }
}



