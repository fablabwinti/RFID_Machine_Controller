#define PIXEL_PIN 1 //GPIO1 = TX pin
#define LED_DEFAULTBRIGTHNESS 6
#define LED_MAXBRIGHTNESS 90  // maximum brightness value for LED (except for 'LED_blink_once' function)
#define LEDUPDATETIME 500  // default led update time in ms

typedef struct {
  uint8_t r;
  uint8_t g;
  uint8_t b;
} RGB;

typedef struct {
  uint8_t h;
  uint8_t s;
  uint8_t v;
} HSV;

Adafruit_NeoPixel LED = Adafruit_NeoPixel(1, PIXEL_PIN, NEO_GRB + NEO_KHZ800);
HSV LEDColor;  // base color of the LED
unsigned long LEDtimestamp;  // timestamp for updating RGB led
bool ledbreathe = 0;  // fade brightness up and down once in current color if set true
bool ledrainbow = 0; //fade throu hue until initial color is reached
bool ledblink = 0;  // blink led in current color brightness while set to one

// HSV to RGB using only integers (error as opposed to using floats is negligeable, maximum 2LSB)
RGB HSVtoRGB(HSV hsvcolor) {
  RGB returncolor;
  uint16_t p, q, t, i, f, h, s, v;
  h = hsvcolor.h;
  s = hsvcolor.s;
  v = hsvcolor.v;
  if (s == 0) {
    returncolor.r = returncolor.g = returncolor.b = v;
  } else {
    h = h << 4;
    i = h / 680;
    f = (h % 680) >> 4;
    p = (v * (255 - s)) >> 8;
    q = (v * ((10710 - (s * f)) / 42)) >> 8;
    t = (v * ((10710 - (s * (42 - f))) / 42)) >> 8;

    switch (i) {
      case 1:
        returncolor.r = q;
        returncolor.g = v;
        returncolor.b = p;
        break;
      case 2:
        returncolor.r = p;
        returncolor.g = v;
        returncolor.b = t;
        break;
      case 3:
        returncolor.r = p;
        returncolor.g = q;
        returncolor.b = v;
        break;
      case 4:
        returncolor.r = t;
        returncolor.g = p;
        returncolor.b = v;
        break;
      case 5:
        returncolor.r = v;
        returncolor.g = p;
        returncolor.b = q;
        break;
      default:  // in case i > 5 or i == 0
        returncolor.r = v;
        returncolor.g = t;
        returncolor.b = p;
        break;
    }
  }

  return returncolor;
}

//increse, then decrease brightness of current color on each call
void LED_breathe(void) {
  static bool increase = true;
  if (increase) {
    LEDColor.v += 2;
    if (LEDColor.v > LED_MAXBRIGHTNESS)
      increase = false;
  } else {
    LEDColor.v -= 2;
    if (LEDColor.v <= LED_DEFAULTBRIGTHNESS) {
      LEDColor.v = LED_DEFAULTBRIGTHNESS;
      increase = true;
      ledbreathe = false;
    }
  }
}


void LED_rainbow(void) {
  static uint8_t starthue =  LEDColor.h;
  ledrainbow = true;

  LEDColor.h += 1;
  if (LEDColor.h == starthue) {
    ledrainbow = false;
  }

}


// sets the LED to the color once, it returns to the default color after 'LEDUPDATETIME' has elapsed
void LED_blink_once(HSV blinkcolor) {
  RGB colortoset;
  LEDtimestamp = millis();
  colortoset = HSVtoRGB(blinkcolor);
  LED.setPixelColor(0, LED.Color(colortoset.r, colortoset.g, colortoset.b));
  LED.show();
}

// update RGB LED
void updateLED(void) {
  RGB colortoset;
  bool updatelednow = false;

  if ((millis() - LEDtimestamp) > 30 && ledbreathe)  // update led every 30 ms
  {
    LEDtimestamp = millis();
    LED_breathe();
    updatelednow = true;
  }
  else if ((millis() - LEDtimestamp) > 10 && ledrainbow)  // update led every 10 ms
  {
    LEDtimestamp = millis();
    LED_rainbow();
    updatelednow = true;
  }
  else if ((millis() - LEDtimestamp) > LEDUPDATETIME) {
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
    colortoset = HSVtoRGB(LEDColor);
    LED.setPixelColor(0, LED.Color(colortoset.r, colortoset.g, colortoset.b));
    LED.show();
  }
}

void LEDinit(void)
{
  pinMode(PIXEL_PIN, OUTPUT);
  LED.begin();  // initialize NeoPixel Library
  // set initial LED color in HSV color space
  LEDColor.h = 0;  // red
  LEDColor.s = 255;  //full saturation
  LEDColor.v = LED_DEFAULTBRIGTHNESS;  // default brightness
  updateLED();  // Initialize LED color
}

/*fastled stuff



#define FASTLED_ESP8266_RAW_PIN_ORDER
#include <FastLED.h>

#define COLOR_ORDER GRB
#define CHIPSET     WS2812B
#define NUM_LEDS    1
CHSV ledcolor_hsv; //a hsv color
CRGB leds[NUM_LEDS]; //data array for the RGB colors (these are sent out to the leds)

  FastLED.addLeds<CHIPSET, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );


  
  ledcolor_hsv.h = 0;//red
  ledcolor_hsv.s = 255;
  ledcolor_hsv.v = 255;

      fill_solid(leds, NUM_LEDS, ledcolor_hsv);
  FastLED.show(); // display this frame

 
 */

