#define PIXEL_PIN 1 //GPIO1 = TX pin
#define LED_DEFAULTBRIGTHNESS 140
#define LED_MAXBRIGHTNESS 250  // maximum brightness value for LED (except for 'LED_blink_once' function)
#define LEDUPDATETIME 50  //led update time in ms (not updated faster than this but can be slower when function is not called that often)

#define COLOR_ORDER GRB
#define CHIPSET     WS2812B
#define NUM_LEDS    1


//LED HSV color
CHSV LEDColor; //a hsv color
CRGB leds[NUM_LEDS]; //data array for the RGB colors (these are sent out to the leds)

unsigned long LEDtimestamp;  // timestamp for updating RGB led
bool ledblink = 0;  // blink led in current color an full brightness while set to one  todo: implement it?


// sets the LED to the color once, it returns to the default color after 'LEDUPDATETIME' has elapsed (or next time updateLED is called, whichever is longer)
void LED_blink_once(uint8_t blinkhue) {
  CHSV blinkcolor; //a hsv color
  blinkcolor.h = blinkhue; //change color of LED to green
  blinkcolor.s = 255;
  blinkcolor.v = LED_MAXBRIGHTNESS;
  LEDtimestamp = millis();
  fill_solid(leds, NUM_LEDS, blinkcolor);
  FastLED.show(); // display this frame

}

// update RGB LED
void updateLED(void) {
  bool updatelednow = false;

  if ((millis() - LEDtimestamp) > LEDUPDATETIME) {
    LEDtimestamp = millis();
    if (ledblink) {
      if (LEDColor.v < LED_MAXBRIGHTNESS)
        LEDColor.v = LED_MAXBRIGHTNESS;
      else
        LEDColor.v = LED_DEFAULTBRIGTHNESS;
    }
    updatelednow = true;
  }
  if (updatelednow)
  {
    fill_solid(leds, NUM_LEDS, LEDColor);
    FastLED.show(); // display this frame
  }
}

void LEDinit(void)
{
  pinMode(PIXEL_PIN, OUTPUT);
  FastLED.addLeds<CHIPSET, PIXEL_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  // set initial LED color in HSV color space
  LEDColor.h = 0;  // red
  LEDColor.s = 255;  //full saturation
  LEDColor.v = LED_DEFAULTBRIGTHNESS;  // default brightness
  updateLED();  // Initialize LED color
}



