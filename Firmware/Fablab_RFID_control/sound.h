

//output pin

#define SOUNDPIN 15//debug!!! use pin 3 //3 //GPIO3 = RX pin

void playTone(int _pin, int _frequency, int _length){
  analogWriteFreq(_frequency);
  analogWrite(_pin, 512);
  delay(_length);
  analogWrite(_pin, 0);
}

// playTone(3,2000,1000); //play a tone on USART RX pin

void playSuccess(void)
{
  playTone(SOUNDPIN,880,150);
  playTone(SOUNDPIN,1760,100);
}

void playFail(void)
{
  playTone(SOUNDPIN,784,200);
  playTone(SOUNDPIN,587,200);
  playTone(SOUNDPIN,523,500);
}

void playBeep(void)
{
  playTone(SOUNDPIN,784,100);;
}
