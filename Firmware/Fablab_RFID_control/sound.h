//beeping sounds for piezo sounder

//output pin
#define SOUNDPIN 3 //GPIO3 = RX pin

void playTone(int _pin, int _frequency, int _length) {
  pinMode(_pin, OUTPUT);
  analogWriteFreq(_frequency);
  analogWrite(_pin, 512);
  delay(_length);
  digitalWrite(SOUNDPIN, LOW);
  ESP.wdtFeed(); //kick hardware watchdog
  delay(5);
}

// playTone(3,2000,1000); //play a tone on USART RX pin

void playLogin(void)
{
  playTone(SOUNDPIN, 880, 150);
  playTone(SOUNDPIN, 1760, 100);
}


void playLogout(void)
{
  playTone(SOUNDPIN, 1760, 150);
  playTone(SOUNDPIN, 880, 100);
}

void playDenied(void)
{
  playTone(SOUNDPIN, 784, 200);
  playTone(SOUNDPIN, 587, 200);
  playTone(SOUNDPIN, 523, 500);
}

void playBeep(void)
{
  playTone(SOUNDPIN, 784, 100);;
}

//plays the first notes of the imperial march
void playMarch(void)
{
  playTone(SOUNDPIN, 440, 500);
  playTone(SOUNDPIN, 440, 500);
  playTone(SOUNDPIN, 440, 500);
  playTone(SOUNDPIN, 349, 350);
  playTone(SOUNDPIN, 523, 150);
  playTone(SOUNDPIN, 440, 500);
  playTone(SOUNDPIN, 349, 350);
  playTone(SOUNDPIN, 523, 150);
  playTone(SOUNDPIN, 440, 650);
  delay(250);
  playTone(SOUNDPIN, 659, 500);
  playTone(SOUNDPIN, 659, 500);
  playTone(SOUNDPIN, 659, 500);
  playTone(SOUNDPIN, 698, 350);
  playTone(SOUNDPIN, 523, 150);
  playTone(SOUNDPIN, 415, 500);
  playTone(SOUNDPIN, 349, 350);
  playTone(SOUNDPIN, 523, 150);
  playTone(SOUNDPIN, 440, 650);
  delay(150);

}
