//digital output for relay and optocoupler

#define OUTPUTPIN 15 //GPIO15
#define POSTLOGOUTDELAY 120000 //time in ms until the relay is switched after user logout

uint32_t userLogoutMillis; //timestamp at end of machine use, can be used to apply post-logout delay of machine locking (when locking the machine switches it off)

//user verified, release the machine for use
void releaseMachine(void)
{
  digitalWrite(OUTPUTPIN, HIGH);
  machineLocked = false;
  LEDColor.h = 0; //red
  userStarttime = getRtcTimestamp();
  userLogoutMillis = 0;
  delay(50);
}

//user logging out, lock the machine
void lockMachine(void)
{
  machineLocked = true;
  LEDColor.h = 87; //green
  enableWifi(); //need to re-enable wifi upon logout

  //todo: display the time the machine was used for some time, also set some post-logout delay here
  userLogoutMillis = millis(); //initi

}

void checkPostLogoutDelay(void)
{
  if (userLogoutMillis > 0) //if true, user has logged out and machine is still enabled
  {
    if (millis() > (userLogoutMillis + POSTLOGOUTDELAY))
    {
      digitalWrite(OUTPUTPIN, LOW);
      userLogoutMillis = 0;
    }
  }
}

void initOutput(void)
{
  pinMode(OUTPUTPIN, OUTPUT);
  userLogoutMillis = 0;
  lockMachine();
}
