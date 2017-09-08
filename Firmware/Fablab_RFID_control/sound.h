//beeping sounds for piezo sounder

//output pin
#define SOUNDPIN 3 //GPIO3 = RX pin

void playTone(int _pin, int _frequency, int _length){
  pinMode(_pin, OUTPUT);
  analogWriteFreq(_frequency);
  analogWrite(_pin, 512);
  delay(_length);
  digitalWrite(SOUNDPIN,LOW);
}

// playTone(3,2000,1000); //play a tone on USART RX pin

void playLogin(void)
{
  playTone(SOUNDPIN,880,150);
  playTone(SOUNDPIN,1760,100);
}


void playLogout(void)
{
  
  playTone(SOUNDPIN,1760,150);
  playTone(SOUNDPIN,880,100);
}

void playDenied(void)
{
  playTone(SOUNDPIN,784,200);
  playTone(SOUNDPIN,587,200);
  playTone(SOUNDPIN,523,500);
}

void playBeep(void)
{
  playTone(SOUNDPIN,784,100);;
}
