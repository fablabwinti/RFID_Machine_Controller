#define OUTPUTPIN 15 //GPIO15

void releaseMachine(void)
{
  digitalWrite(OUTPUTPIN, HIGH);
}

void lockMachine(void)
{
  digitalWrite(OUTPUTPIN, LOW);
}

void initOutput(void)
{
  pinMode(OUTPUTPIN, OUTPUT);
  lockMachine();
}
