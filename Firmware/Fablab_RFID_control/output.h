//digital output for relay and optocoupler

#define OUTPUTPIN 15 //GPIO15

//user verified, release the machine for use
void releaseMachine(void)
{
  digitalWrite(OUTPUTPIN, HIGH);
  machineLocked = false;
  LEDColor.h = 0; //red
  userStarttime = getRtcTimestamp();
  postlogoutmillis = 0;
  delay(50);
}

//user logging out, lock the machine
void lockMachine(void)
{
  machineLocked = true;
  LEDColor.h = 87; //green
  enableWifi(); //need to re-enable wifi upon logout
  userStoptime = getRtcTimestamp();
  postlogoutmillis = millis();
}

//call this function in main loop to disable machine after logout, only call it if the user is not logged in!
void checkPostLogoutDelay(void)
{
  if (postlogoutmillis > 0)
  {
    if (millis() > postlogoutmillis + (long)config.mSwitchoffDelay * 1000)
    {
      digitalWrite(OUTPUTPIN, LOW); //disable output
      if(millis() > postlogoutmillis + 30000) //after at least 30 seconds (or after switching), disable checking, allows for displaying post logout info even without delayed switching
      {
        postlogoutmillis = 0;
      }
    }
  }
}

void initOutput(void)
{
  pinMode(OUTPUTPIN, OUTPUT);
  digitalWrite(OUTPUTPIN, LOW); //disable output
  lockMachine();
  postlogoutmillis = 0;
}
