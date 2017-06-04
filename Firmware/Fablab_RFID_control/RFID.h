/*
   card UID is 4 bytes
   one data sector contains 16 bytes of user data
   TBD if using two sectors for user name or only one, depens on reading speed

 * */
//Pin definitions
#define RST_PIN         255        //RST not connected, use soft reset
#define SS_PIN          2         //chip select pin

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance

//authenticate and read one block (16bytes+2bytes) of the RFID card (blockbuffer needs to be at least 18 bytes)
boolean getRFIDdata(MFRC522::MIFARE_Key *key, uint8_t block, uint8_t* RFIDblockbuffer)
{
  boolean result = false;
  MFRC522::StatusCode status;

  // http://arduino.stackexchange.com/a/14316   -> BUT: if these two commands are executed twice before authentication, it fails with a timeout (commands are executed during card detect already)
  //    if ( ! mfrc522.PICC_IsNewCardPresent())
  //        return false;
  //    if ( ! mfrc522.PICC_ReadCardSerial())
  //        return false;
  // Serial.println(F("Authenticating using key A..."));
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("PCD_Authenticate() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return false;
  }

  // Read block
  byte byteCount = 18; //get 18 bytes of data
  status = mfrc522.MIFARE_Read(block, RFIDblockbuffer, &byteCount);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("MIFARE_Read() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
  }
  else {
    // Successful read
    result = true;
    Serial.println(F("Success"));

  }
  Serial.println();


  return result;
}

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
  //todo: add display error, add alarm error

  Serial.println(F("AUTHENTICATION FAILED, ACCESS DENIED!"));
    playFail();
}


void authenticationSuccess(void)
{
  //todo: add display error, add alarm error
  
  Serial.println(F("AUTHENTICATED, ACCESS GRANTED"));
  playSuccess();
  
}




//verify the UID exists in the database, check if the card is valid (correct key and a known user), unlock machine if valid
void verifyRFIDdata(byte *uidbuffer, byte uidsize) {

  if (uidsize < 4)
  {
    authenticationFail();
    return;
  }

  //todo: what happens if the UID is bigger? which bytes are taken? is this even an issue? probably not...
  uint32_t uid = uidbuffer[0] | (uidbuffer[1]<<8) | (uidbuffer[2]<<16) | (uidbuffer[3]<<24); //direct typecast is dangerous due to memory alignment, better to do it this way

  Serial.println(uid, DEC);
  
  if(userDBfindentry(uid)) 
  {
    authenticationSuccess();
  }
  else
  {
    //debug: add the entry
  //  char n1[16] = {0}; 
  //  char n2[16] = {0};
  //  userDBaddentry(uid, n1, n2);
    authenticationFail();
  }
}



//check if an RFID card is present and act on it (i.e. verify with the database, unlock the machine if verified)
void checkRFID(void)
{
  // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;  //todo: could set the key in the config, make it changeable on the webpage or something

  //some variables we need
  byte block;
  byte len;
  MFRC522::StatusCode status;

  //-------------------------------------------

  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  Serial.println(F("**Card Detected:**"));

  //-------------------------------------------

  mfrc522.PICC_DumpDetailsToSerial(&(mfrc522.uid)); //dump some details about the card
  //mfrc522.PICC_DumpToSerial(&(mfrc522.uid));      //all blocks in hex

  //-------------------------------------------

  Serial.print(F("Name: "));

  byte namebuffer[36];

  block = 1;
  len = 18;
  getRFIDdata(&key, block, namebuffer);


  block = 2;
  getRFIDdata(&key, block, &namebuffer[18]);

  verifyRFIDdata(mfrc522.uid.uidByte, mfrc522.uid.size);

  mfrc522.PICC_HaltA();       // Halt PICC
  mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD


  //PRINT NAME
  for (uint8_t i = 0; i < 36; i++) {
    Serial.write(namebuffer[i]);
  }
  Serial.println("");



}


void initRFID(void)
{
  mfrc522.PCD_Init();
}

