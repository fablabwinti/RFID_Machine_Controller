


void WriteStringToEEPROM(int beginaddress, String str, uint8_t maxlength)
{
  if (str.length() > maxlength)  str.remove(maxlength); //truncate string if its too long
  char  charBuf[str.length() + 1];
  str.toCharArray(charBuf, str.length() + 1);
  for (int t =  0; t < sizeof(charBuf); t++)
  {
    EEPROM.write(beginaddress + t, charBuf[t]);
  }
}

String  ReadStringFromEEPROM(int beginaddress)
{
  byte counter = 0;
  char rChar;
  String retString = "";
  while (1)
  {
    rChar = EEPROM.read(beginaddress + counter);
    if (rChar == 0) break;
    if (counter > 60) break;
    counter++;
    retString.concat(rChar);

  }
  return retString;
}

void EEPROMWritelong(int address, long value)
{
  byte four = (value & 0xFF);
  byte three = ((value >> 8) & 0xFF);
  byte two = ((value >> 16) & 0xFF);
  byte one = ((value >> 24) & 0xFF);

  //Write the 4 bytes into the eeprom memory.
  EEPROM.write(address, four);
  EEPROM.write(address + 1, three);
  EEPROM.write(address + 2, two);
  EEPROM.write(address + 3, one);
}
long EEPROMReadlong(long address)
{
  //Read the 4 bytes from the eeprom memory.
  long four = EEPROM.read(address);
  long three = EEPROM.read(address + 1);
  long two = EEPROM.read(address + 2);
  long one = EEPROM.read(address + 3);

  //Return the recomposed long by using bitshift.
  return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
}

void EEPROMWriteByteArray(int address, uint8_t *value, int datalength)
{
  int i;
  for (i = 0; i < datalength; i++)
  {
    EEPROM.write(address + i, value[i]);
  }
}

void EEPROMReadByteArray(int address, uint8_t *value, int datalength)
{
  int i;
  for (i = 0; i < datalength; i++)
  {
    value[i] = EEPROM.read(address + i);
  }
}

// convert a single hex digit character to its integer value (from https://code.google.com/p/avr-netino/)
unsigned char h2int(char c)
{
  if (c >= '0' && c <= '9') {
    return ((unsigned char)c - '0');
  }
  if (c >= 'a' && c <= 'f') {
    return ((unsigned char)c - 'a' + 10);
  }
  if (c >= 'A' && c <= 'F') {
    return ((unsigned char)c - 'A' + 10);
  }
  return (0);
}

//decode string from url-arguments
String urldecode(String input) // (based on https://code.google.com/p/avr-netino/)
{
  char c;
  String ret = "";

  for (byte t = 0; t < input.length(); t++)
  {
    c = input[t];
    if (c == '+') c = ' ';
    if (c == '%') {


      t++;
      c = input[t];
      t++;
      c = (h2int(c) << 4) | h2int(input[t]);
    }

    ret.concat(c);
  }
  return ret;
}

//split an input string at a separator, returns part before the separator (maximum sting length: 255)
String splitStringbySeparator(String inputstr, char separator)
{
  uint8_t i,j;
  j = inputstr.length();
  for(i=0; i<j; i++)
  {
    if(inputstr.charAt(i)==separator)
    {
      return inputstr.substring(0, i);
    }
  }
  return inputstr; //not found, return full string
}

