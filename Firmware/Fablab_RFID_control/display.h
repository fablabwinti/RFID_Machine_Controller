#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSans9pt7b.h>  //linienabstand: 16
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSans18pt7b.h>
#include <Fonts/FreeSansBoldOblique12pt7b.h>
#include <Fonts/FreeSansBold18pt7b.h>
#include <Fonts/FreeSansBold12pt7b.h>
#include <Fonts/FreeMonoBold12pt7b.h>

Adafruit_SSD1306 display(0); //do not use reset pin

#if (SSD1306_LCDHEIGHT != 64)
#error("Display height not set to 64 pixels, please fix Adafruit_SSD1306.h!");
#endif



void displayUpdate(void) {

//todo: need to define the display layout

//stuff to display: time
//currently logged in user 

//if machine is unused: 
//display the current time, maybe also the date, what else?

if(machineLocked)
{
    display.setFont(&FreeSans9pt7b);
    display.setCursor(12, 5);
    display.print('Mensch Meyer...');
    display.setFont(&FreeSansBold18pt7b);
    display.setCursor(20, 2);
    display.print('LOCKED');
}
else
{
    display.setFont(&FreeSansBold12pt7b);    
    display.setCursor(5, 40);
    display.print(' 1:22:34');
    display.setCursor(20, 2);
    display.print('FREE');
}
  
  
  char numberbuffer[6];
  dtostrf(1234, 6, 2, numberbuffer);
  //display.print(numberbuffer);

}



void displayinit(void)
{
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  Wire.setClock(400000);  // use fast speed
  display.clearDisplay();
  display.display();  //
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setTextWrap(false);

//todo: add fablab logo here
  display.setCursor(29, 15);
  display.setFont(&FreeSansBold12pt7b);
  display.println("Fablab");
  display.setCursor(36, 54);
  display.print("Winti");
  display.display();
}

