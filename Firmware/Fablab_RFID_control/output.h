#define OUTPUTPIN 15 //GPIO15

//user verified, release the machine for use
void releaseMachine(void)
{
  digitalWrite(OUTPUTPIN, HIGH);
  machineLocked = false;
  LEDColor.h = 0; //red
  userStarttime = getRtcTimestamp();

  //todo: add sound here?
}

void lockMachine(void)
{
  digitalWrite(OUTPUTPIN, LOW);
  machineLocked = true;
  LEDColor.h = 87; //green
  //todo: add sound here?
}

void initOutput(void)
{
  pinMode(OUTPUTPIN, OUTPUT);
  lockMachine();
}
