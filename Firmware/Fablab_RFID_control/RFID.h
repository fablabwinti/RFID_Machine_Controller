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
//this function is called by verifyRFIDdata()     //note: reading of two data blocks takes about 15ms
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
  uint8_t block = (sector * 4) + 3; //trailer blok for the sector (each sector has 4 blocks, the last one is the trailer)
  //create the access configuration buffer
  uint8_t trailerbuffer[16];
  uint8_t i;
  for (i = 0; i < 6; i++)
  {
    trailerbuffer[i] = newkey[i]; //copy to key A
    trailerbuffer[i + 10] = newkey[i] + 1; //copy to key B (key B can also be used to access the data, so make it almost the same, key B can only be read if key A is known so this is safe)
  }
  //set the access bits (001 for trailer, 000 for data blocks, see datasheet for details)
  trailerbuffer[6] = 0xFF;
  trailerbuffer[7] = 0x07;
  trailerbuffer[8] = 0x80;

  // Write block of 16 bytes to the trailer
  return writeRFIDdata(key, block, trailerbuffer);
}

//update a blank card to the KEY stored in the config. also update the contents of block 4 to the contents stored in the config.
//the contents of block 4 (sector 1) is used to verify the card, if the contents or the key is wrong, the card is not accepted (much safer than just using the UID which can be copied easily on writeable cards)
//this function must only be run if a card is present
//WARNING: if the key stored in the conifg is not known this function can brick your card i.e. you cannot access the sector 1 ever again unless you find the correct access key.
void programRFIDkeys(void)
{
  MFRC522::MIFARE_Key key; //key for data access of the card (each sector of 4x16 bytes can have a separate key)
  uint8_t sector = 1;
  uint8_t i;

  //use the flag 'RFIDtagprogrogramming' to determine whether to program a blank tag or to blank out a programmd tag (for reprogramming later).
  //if flag == 1 -> program, if flag == 2 -> clear tag storage
  if (RFIDtagprogrogramming == 1) //program tag
  {
    if (websocket_connected)
    {
      String info = String(F("Programming tag..."));
      WS_print(info);
    }
    //first, access sector 1 trailer (block7) using the default key (0xFFFFFFFFFFFF)
    for (i = 0; i < 6; i++) key.keyByte[i] = 0xFF;
    if (changeKeyA(&key, sector, config.RFIDkey)) //write the new access key stored in the config
    {
      //now write the code into block 4 using the newly written key to access
      for (i = 0; i < 6; i++) key.keyByte[i] = config.RFIDkey[i];
      Serial.println(F("Writing block 4 using NEW key .... "));
      if (writeRFIDdata(&key, sector * 4, config.RFIDcode))
      {
        //if successful, verify the data
        uint8_t databuffer[18]; //need 18byte buffer for the 16 data bytes (two additional for command. its kind of a stupid implementation in the lib)
        if (getRFIDdata(&key, sector * 4, (uint8_t*)databuffer)) //read the buffer
        {
          uint8_t checkcount = 0;
          //verify if the buffer contents is correct
          for (uint8_t i = 0; i < 16; i++)
          {
            if (databuffer[i] == config.RFIDcode[i])
            {
              checkcount++;
            }
          }
          if (checkcount == 16)
          {
            if (websocket_connected)
            {
              String info = String(F("OK"));
              WS_println(info);
            }
            playLogin();
            Serial.println(F("Code Verified, tag is written"));
            return;
          }
        }
      }
    }
  }
  else if (RFIDtagprogrogramming == 2) //clear tag
  {
    if (websocket_connected)
    {
      String info = String(F("Clearing tag..."));
      WS_print(info);
    }
    //first, access sector 1 trailer (block7) using the config key
    uint8_t blankdata[16];
    for (i = 0; i < 16; i++) blankdata[i] = 0xFF;
    for (i = 0; i < 6; i++) key.keyByte[i] = config.RFIDkey[i];
    if (changeKeyA(&key, sector, blankdata)) //write blank key
    {
      //now write blank code into block 4
      for (i = 0; i < 6; i++) key.keyByte[i] = 0xFF;
      Serial.println(F("Writing block 4 using blank key .... "));
      if (writeRFIDdata(&key, sector * 4, blankdata))
      {
        if (websocket_connected)
        {
          String info = String(F("OK"));
          WS_println(info);
        }
        playLogin();
        Serial.println(F("tag is now blank"));
        return;

      }
    }

  }
  //if unsuccessful, play fail tone
  if (websocket_connected)
  {
    String info = String(F("FAIL"));
    WS_println(info);
  }
  playDenied(); //access is denied
}

//reasons: 0 = unauthorized, 1 = occupied, 2 = invalid tag
void authenticationFail(uint8_t reason)
{
  displayDenied(reason);
  Serial.println(F("ACCESS DENIED!"));
  playDenied();
  delay(500); //wait while displaying the reason
}


void authenticationSuccess(void)
{
  displayLogin();
  Serial.println(F("AUTHENTICATED, ACCESS GRANTED"));
  playLogin();
  releaseMachine();
  sendPendingEvents(true);  // send data out immediately (enforced sendout) (no data is sent while user is logged in)
  delay(500); //wait while displaying login
}




//verify the UID exists in the database, check if the card is valid (correct key and a known user), unlock machine if valid
//todo: add name verification?
void verifyRFIDdata() {

  MFRC522::MIFARE_Key key; //key for data access of the card (each sector of 4x16 bytes can have a separate key)

  if (mfrc522.uid.size < 4)
  {
    authenticationFail(2);
    return;
  }

  //todo: what happens if the UID is bigger? which bytes are taken? is this even an issue? probably not...
  uint32_t uid = mfrc522.uid.uidByte[0] | (mfrc522.uid.uidByte[1] << 8) | (mfrc522.uid.uidByte[2] << 16) | (mfrc522.uid.uidByte[3] << 24); //direct typecast is dangerous due to memory alignment, better to do it this way

  Serial.println(uid, DEC);
  if (websocket_connected)
  {
    String info = String(F("RFID Card detected: UID = "));
    info += String(uid);
    WS_print(info);
  }

  //verify the cards data contents agains the config settings
  for (byte i = 0; i < 6; i++) key.keyByte[i] = config.RFIDkey[i];  //prepare the key

  Serial.print(F("Reading block 4 using NEW key .... "));
  uint8_t databuffer[18]; //need 18byte buffer for the 16 data bytes (two additional for command. its kind of a stupid implementation in the lib)
  uint8_t block = 4;
  if (getRFIDdata(&key, block, (uint8_t*)databuffer)) //read the buffer
  {
    Serial.println(F("key is ok"));
    //verify if the buffer contents is correct
    for (uint8_t i = 0; i < 16; i++)
    {
      if (databuffer[i] != config.RFIDcode[i])
      {
        if (websocket_connected)
        {
          String info = String(F(" code data does not match"));
          WS_println(info);
        }
        addEventToQueue(6, String("uid:") + String(uid) + " code error"); //event 6 = tag_error
        authenticationFail(2);
        return;
      }
    }
  }
  else
  {
    if (websocket_connected)
    {
      String info = String(F(" wrong access key"));
      WS_println(info);
    }
    addEventToQueue(6, String("uid:") + String(uid) + " key error"); //event 6 = tag_error
    authenticationFail(2);
    return;
  }

  //the card is now accepted as being valid, check if the user is allowed to access this machine:

  //check if this is the admin tag
  if (config.adminUID == uid)
  {
    if (websocket_connected)
    {
      String info = String(F(" THIS IS THE ADMIN TAG"));
      playLogin();
      WS_println(info);
    }
    else
    {
      if (machineLocked == false)
      {
        //if machine is running, stop and logout
        displayLogout(); //print logout on the display
        playLogout();
        lockMachine();
        currentuser = 0; //user logged out
        addEventToQueue(5, 0 ,  "Admin logout"); //event 5 = tag_logout
      }
      else
      {
        //if not running, release machine:
        currentuser = 0; //user is admin
        addEventToQueue(4, 0, "Admin login"); //event 4 = tag_login
        authenticationSuccess();
      }
    }
    return;
  }

  uint16_t dbentryno = userDBfindentry(uid); //find the uid in the database, read the user entry into 'userentry' struct



  if (websocket_connected)
  {
    String info = "";
    if (dbentryno > 0)
    {
      info += String(F(" User = ")) + String(userentry.name) + String(F("is authorized to use this machine"));
      displayLogin(); //print login on the display
      playLogin();
    }
    else
    {
      info += String(F(" user not authorized"));
      playDenied();
    }
    WS_println(info);
  }
  else //webserver not active, authenticate the user
  {
    if (dbentryno > 0 && localTimeValid) //if entry found in database and local RTC time is valid (time is needed to veryfy if tag has expired)
    {
      RtcDateTime RTCtime  = RTC.GetDateTime();
      //time verification, check if user's time is already valid and not yet expired
      if (userentry.ts_validfrom > RTCtime.Epoch32Time() || userentry.ts_validuntil < RTCtime.Epoch32Time())
      {
        authenticationFail(0); //access is denied, tag not valid yet or expired (entry is be removed on next database sync)
        addEventToQueue(6, userentry.tagid, "expired"); //event 6 = tag_error
        Serial.println(F("Tag Accepted but tag time invalid"));
        return;
      }


      if (machineLocked == false)
      {
        //if machine is running, check if the user is logging out:
        if (currentuser == dbentryno)
        {
          displayLogout(); //print logout on the display
          playLogout();
          lockMachine();
          currentuser = 0; //user logged out
          addEventToQueue(5, userentry.tagid , String(userentry.name) + " logout"); //event 5 = tag_logout, no need to read userentry from DB, it was read above in the findentry function
          delay(800);
        }
        else
        {
          //if running and not current user, play sound
          authenticationFail(1);
          addEventToQueue(6, userentry.tagid, "occupied"); //event 6 = tag_error
        }
      }
      else
      {
        //if not running, set the current user and release machine:
        currentuser = dbentryno;
        addEventToQueue(4, userentry.tagid, String(userentry.name) + " login"); //event 4 = tag_login, no need to read userentry from DB, it was read above in the findentry function
        authenticationSuccess();
      }
    }
    else //user not authorized for this machine
    {
      //debug: add the entry
      // char n1[16] = {0};
      //char n2[16] = {0};
      //userDBaddentry(uid, n1, n2);
      authenticationFail(0); //access is denied
      addEventToQueue(6, String("uid:") + String(uid) + " denied"); //event 6 = tag_error

    }
  }
}



//check if an RFID card is present and act on it (i.e. verify with the database, unlock the machine if verified)
void checkRFID(void)
{
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  Serial.println(F("Tag Detected"));

  if (RFIDtagprogrogramming == 0)
  {
    //by default, authenticate the tag
    verifyRFIDdata();
  }
  else
  {
    //if requested on the webpage, program blank tags/ blank out programmed tags
    programRFIDkeys(); //function checks the global flag and tries to program/blank a card. it uses sound for feedback.
  }

  mfrc522.PICC_HaltA();       // Halt PICC  //todo: moved this from below to make function return if no new card is detected
  mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD

}




void initRFID(void)
{
  mfrc522.PCD_Init();
  //mfrc522.PCD_SetAntennaGain(0x07 << 4); //increase RX gain to maximum (increases range by about 0.5cm)
}

/*gain settings:
   RxGain_18dB       = 0x00 << 4,  // 000b - 18 dB, minimum
    RxGain_23dB       = 0x01 << 4,  // 001b - 23 dB
    RxGain_18dB_2     = 0x02 << 4,  // 010b - 18 dB, it seems 010b is a duplicate for 000b
    RxGain_23dB_2     = 0x03 << 4,  // 011b - 23 dB, it seems 011b is a duplicate for 001b
    RxGain_33dB       = 0x04 << 4,  // 100b - 33 dB, average, and typical default
    RxGain_38dB       = 0x05 << 4,  // 101b - 38 dB
    RxGain_43dB       = 0x06 << 4,  // 110b - 43 dB
    RxGain_48dB       = 0x07 << 4,  // 111b - 48 dB, maximum
    RxGain_min        = 0x00 << 4,  // 000b - 18 dB, minimum, convenience for RxGain_18dB
    RxGain_avg        = 0x04 << 4,  // 100b - 33 dB, average, convenience for RxGain_33dB
    RxGain_max        = 0x07 << 4   // 111b - 48 dB, maximum, convenience for RxGain_48dB
  };
 */






