/*
   card UID is 4 bytes
   one data sector contains 16 bytes of user data
   TBD if using two sectors for user name or only one, depens on reading speed

  For details about the memory inside the RFID chips check the datasheet: http://cache.nxp.com/documents/data_sheet/MF1S50YYX_V1.pdf

  note: access key bits must be set to 0 0 0 (can never be changed again) or 0 0 1 (use key A only, key A required to read anything) 001 is the factory default, so they do not have to be changed (key B cannot be used as it is readable with key A, see datasheet of MF1S5009)

  note on sectors and blocks: the card is accessed with block numbers. each block contains 16 bytes of data. every fourth block is a 'trailer' block containing access data for the three blocks
  below it. block 3 is the trailer block for sector 0, block 7 is the trailer block for sector 1 and so on.
  to change the access key (and access bits) of sector 1 which is used to (slightly) increase authentication rewrite key A in block 7.

 * */
//Pin definitions
#define RST_PIN         255        //RST not connected, use soft reset
#define SS_PIN          2         //chip select pin

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance

//authenticate and read one block (16bytes) of the RFID card (blockbuffer needs to be at least 18 bytes, as stated in the library)
//make sure the commands PICC_IsNewCardPresent() and PICC_ReadCardSerial() were executed before calling this function
//this function is called by verifyRFIDdata()
boolean getRFIDdata(MFRC522::MIFARE_Key *key, uint8_t block, uint8_t* RFIDblockbuffer)
{
  boolean result = false;
  MFRC522::StatusCode status;

  // Serial.println(F("Authenticating using key A..."));
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("PCD_Authenticate() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return false;
  }

  // Read block
  byte byteCount = 18; //get 16 bytes of data, two extra bytes are needed as the library adds the read command to the buffer), the returned 16 bytes start at buffer[0]
  status = mfrc522.MIFARE_Read(block, RFIDblockbuffer, &byteCount);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("MIFARE_Read() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
  }
  else {
    // Successful read
    result = true;
    //  Serial.println(F("Success"));

  }
  // Serial.println();


  return result;
}

//write one block of 16bytes of data to the RFID card (cannot write less than 16 bytes, access is per block of 16 bytes!)
//make sure the commands PICC_IsNewCardPresent() and PICC_ReadCardSerial() were executed before calling this function
boolean writeRFIDdata(MFRC522::MIFARE_Key *key, uint8_t block, uint8_t* RFIDblockbuffer)
{
  MFRC522::StatusCode status;
  //Serial.println(F("Authenticating using key A..."));
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("PCD_Authenticate() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return false;
  }
  else Serial.println(F("PCD_Authenticate() success: "));

  // Write block of 16 bytes
  status = mfrc522.MIFARE_Write(block, RFIDblockbuffer, 16);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("MIFARE_Write() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return false;
  }
  else Serial.println(F("MIFARE_Write() success: "));

  return true;
}


void authenticationFail(void)
{
  //todo: add display error
  Serial.println(F("ACCESS DENIED!"));
  playDenied();
}


void authenticationSuccess(void)
{
  //todo: add display error
  Serial.println(F("AUTHENTICATED, ACCESS GRANTED"));
  playLogin();
  releaseMachine();
}




//verify the UID exists in the database, check if the card is valid (correct key and a known user), unlock machine if valid
//todo: add name verification?
void verifyRFIDdata(byte *uidbuffer, byte uidsize) {

  if (uidsize < 4)
  {
    authenticationFail();
    return;
  }

  //todo: what happens if the UID is bigger? which bytes are taken? is this even an issue? probably not...
  uint32_t uid = uidbuffer[0] | (uidbuffer[1] << 8) | (uidbuffer[2] << 16) | (uidbuffer[3] << 24); //direct typecast is dangerous due to memory alignment, better to do it this way

  Serial.println(uid, DEC);


  uint16_t dbentryno = userDBfindentry(uid); //find the uid in the database, read the user entry into 'userentry' struct


  if (webserver_active) //webserver acitve, print UID to webpage
  {

    if (websocket_connected)
    {
      String info = String(F("RFID Card detected: UID = "));
      info += String(uid);
      if (dbentryno > 0)
      {
        info += String(F(" User = ")) + String(userentry.name) + String(F("is authorized to use this machine"));
      }
      else
      {
        info += String(F(" user not authorized"));
      }
      WS_println(info);
    }
  }
  else //webserver not active, authenticate the user
  {


    //todo: need to add a verification here, the functuion must be implemented in database.h first (check if user's time is already valid and not yet expired)


    if (dbentryno > 0)
    {
      if (machineLocked == false)
      {
        //if machine is running, check if the user is logging out:
        if (currentuser == dbentryno)
        {
          playLogout();
          lockMachine();
          currentuser = 0; //user logged out
          addEventToQueue(4, userentry.tagid , String(userentry.name) + " logout"); //event 4 = tag_logout, no need to read userentry from DB, it was read above in the findentry function
        }
        else
        {
          //if running and not current user, play sound
          authenticationFail();
        }
      }
      else
      {
        //if not running, set the current user and release machine:
        currentuser = dbentryno;
        authenticationSuccess();
        addEventToQueue(3, userentry.tagid, String(userentry.name) + " login"); //event 3 = tag_login, no need to read userentry from DB, it was read above in the findentry function
        sendPendingEvents(true);  // send data out immediately (enforced sendout) (no data is sent while user is logged in)
      }
    }
    else //user not authorized for this machine
    {
      //debug: add the entry
      // char n1[16] = {0};
      //char n2[16] = {0};
      //userDBaddentry(uid, n1, n2);
      authenticationFail(); //access is denied
    }
  }
}



//check if an RFID card is present and act on it (i.e. verify with the database, unlock the machine if verified)
void checkRFID(void)
{
  MFRC522::MIFARE_Key key; //key for data access of the card (each sector of 4x16 bytes can have a separate key)
  byte block;
  MFRC522::StatusCode status;

  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  Serial.println(F("Card Detected"));
  //-------------------------------------------
  // mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid)); //dump some details about the card
  //mfrc522.PICC_DumpToSerial(&(mfrc522.uid));      //all blocks in hex
  //-------------------------------------------

  // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;  //todo: could set the key in the config, make it changeable on the webpage or something

  // *((uint32_t*)key.keyByte) = RFID_SECTORKEY;   //access key for sector 1 NO!!!! cannot write 8 bytes to 6 byte array!!!
  uint8_t databuffer[18]; //16byte databuffer for secret key stored in sector 1 (plus 2 commans bytes)
  uint32_t needtomatch[4] = RFID_SECRETKEY; //16byte databuffer for secret key stored in sector 1 (block 4)



  //  block = 1;
  //  getRFIDdata(&key, 1, (uint8_t*)databuffer);
  //  block = 2;
  //  getRFIDdata(&key, block, (uint8_t*)databuffer);
  //note: reading of two blocks takes about 15ms

  verifyRFIDdata(mfrc522.uid.uidByte, mfrc522.uid.size); //todo: add name verification?

  mfrc522.PICC_HaltA();       // Halt PICC  //todo: moved this from below to make function return if no new card is detected
  mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD

  //Serial.print(F("Name: "));
  //print buffer contents, char decoded
  //for (uint8_t i = 0; i < 36; i++) {
  //  Serial.write(namebuffer[i]);
  //}
  // Serial.println("");

}




void initRFID(void)
{
  mfrc522.PCD_Init();
}


//update the default key for sector 1:
//get access to block 7 (trailer for sector 1) -> default config bits are 0,0,0 (or they should be) meaning key A has access to data and trailer, key B only to the trailer -> key B is not used (it is intended for more, double secure access, which we do not need)
//write the first 6 bytes of this sector with a new key
//

//change the key of a sector (can be 0 to 15). the  "key" is the current access key, the 'newkey' buffer must contain 6 bytes with the new key which is written to the trailer of the sector
//the access bits are set to 001 for the trailer and to 000 for data blocks (access only with keyA, key B is unused, access bits can be read and written using key A) calculation -> https://www.mifare.net/support/forum/topic/what-is-mifare-classic-1k-access-bits-means-how-to-calculate-and-use-it/
//0xFF 0x07 0x80 0xXX  (XX is the user byte and can be anything)
//key B also has to be changed as access to the data is possible using key B. make both keys the same, key B can be read but only if key A is known
//make sure the commands PICC_IsNewCardPresent() and PICC_ReadCardSerial() were executed before calling this function
bool changeKeyA(MFRC522::MIFARE_Key *key, uint8_t sector, uint8_t* newkey)
{

  MFRC522::StatusCode status;
  uint8_t block = (sector * 4) + 3; //trailer blok for the sector (each sector has 4 blocks, the last one is the trailer)

  //Serial.println(F("Authenticating using key A..."));
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("PCD_Authenticate() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return false;
  }
  else Serial.println(F("PCD_Authenticate() success: "));

  //create the access configuration buffer
  uint8_t trailerbuffer[16];
  uint8_t i;
  for (i = 0; i < 6; i++)
  {
    trailerbuffer[i] = newkey[i]; //copy to key A
    trailerbuffer[i + 10] = newkey[i]+1; //copy to key B (key B can also be used to access the data, so make it almost the same, key B can only be read if key A is known so this is safe)
  }
  //set the access bits
  trailerbuffer[6] = 0xFF;
  trailerbuffer[7] = 0x07;
  trailerbuffer[8] = 0x80;
 

  // Write block of 16 bytes to the trailer

  status = mfrc522.MIFARE_Write(block, trailerbuffer, 16);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("MIFARE_Write() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return false;
  }
  else Serial.println(F("MIFARE_Write() success: "));

  return true;

}


//debug function to test reading&writing data to rfid card safely
//reads & writes block 4 (first block of sector 1) and changes access to that block
void rfidsecuretest(void)
{

  MFRC522::MIFARE_Key key; //key for data access of the card (each sector of 4x16 bytes can have a separate key)
  byte block;
  byte len;
  MFRC522::StatusCode status;

  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  Serial.println(F("Card Detected"));
  //-------------------------------------------
  // mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid)); //dump some details about the card
  //mfrc522.PICC_DumpToSerial(&(mfrc522.uid));      //all blocks in hex
  //-------------------------------------------

  // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;  //todo: could set the key in the config, make it changeable on the webpage or something

  uint8_t databuffer[18]; //databuffer for secret key stored in block 4
  char needtomatch[] = "Das ist ein Test"; //16byte databuffer for secret key stored in sector 1
/*
  Serial.print(F("Reading block 4 using default key .... "));

  //  len = 18; //need to read 18 bytes (did not investigate what the two additional bytes are, maybe a CRC?)
  //read block 1 and block 2 (16 data bytes each)
  block = 4;
  if (getRFIDdata(&key, block, (uint8_t*)databuffer))
  {
    Serial.print(F("OK: "));
    for (uint8_t i = 0; i < 16; i++)
    {
      Serial.write(databuffer[i]);
    }
  }
  else
  {
    Serial.println(F("FAIL"));
  }

  Serial.print(F("Writing block 4 using default key .... "));

  //  len = 18; //need to read 18 bytes (did not investigate what the two additional bytes are, maybe a CRC?)
  //read block 1 and block 2 (16 data bytes each)
  block = 4;
  if (writeRFIDdata(&key, block, (uint8_t*)needtomatch))
  {
    Serial.println(F("OK"));
  }
  else
  {
    Serial.println(F("FAIL"));
  }

  Serial.print(F("Changing the key of sector 1 now using default key..."));
  */
  uint8_t key1[6]; //new 6 byte key

  key1[0] = 'A';
  key1[1] = 'B';
  key1[2] = 'C';
  key1[3] = 'D';
  key1[4] = 'E';
  key1[5] = 'F';
  //ABCDEF in hex ist: (rückwärts, LSB = A) 0x464544434241

  for (byte i = 0; i < 6; i++) key.keyByte[i] = config.RFIDkey[i];  //todo: could set the key in the config, make it changeable on the webpage or something
  
/*
  if (changeKeyA(&key, 1, key1)) //change the key
  {
    Serial.println(F("OK"));
  }
  else
  {
    Serial.println(F("FAIL"));
  }

*/

  Serial.print(F("Reading block 4 using NEW key .... "));




  //  len = 18; //need to read 18 bytes (did not investigate what the two additional bytes are, maybe a CRC?)
  //read block 1 and block 2 (16 data bytes each)
  block = 4;
  if (getRFIDdata(&key, block, (uint8_t*)databuffer))
  {
    Serial.print(F("OK: "));
    for (uint8_t i = 0; i < 16; i++)
    {
      Serial.write(databuffer[i]);
    }
  }
  else
  {
    Serial.println(F("FAIL"));
  }

  Serial.print(F("Writing block 4 using NEW key .... "));

  //  len = 18; //need to read 18 bytes (did not investigate what the two additional bytes are, maybe a CRC?)
  //read block 1 and block 2 (16 data bytes each)
  block = 4;
  if (writeRFIDdata(&key, block, config.RFIDcode))
  {
    Serial.println(F("OK"));
  }
  else
  {
    Serial.println(F("FAIL"));
  }


  mfrc522.PICC_HaltA();       // Halt PICC  //todo: moved this from below to make function return if no new card is detected
  mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD







}






