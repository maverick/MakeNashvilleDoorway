#include "LPD8806.h"
#include "SPI.h" // Comment out this line if using Trinket or Gemma
#ifdef __AVR_ATtiny85__
 #include <avr/power.h>
#endif

// Example to control LPD8806-based RGB LED Modules in a strip

/*****************************************************************************/

// Number of RGB LEDs in strand:
int nLEDs = 286;

// We are using hardware SPI for faster writes, so we leave out
// the data and clock pin parameters.  This does limit use to very
// specific pins on the Arduino.  For "classic" Arduinos (Uno, Duemilanove,
// etc.), data = pin 11, clock = pin 13.  For Arduino Mega, data = pin 51,
// clock = pin 52.  For 32u4 Breakout Board+ and Teensy, data = pin B2,
// clock = pin B1.  For Leonardo, this can ONLY be done on the ICSP pins.
LPD8806 strip = LPD8806(nLEDs);

// Starting positions of each segment
int LeftStart  = 0;
int TopStart   = 96;
int RightStart = 190;

// Let's define some standard colors so we don't have to repeat this lots of times in the code
uint32_t RED    = strip.Color(127,  0,  0);
uint32_t BLUE   = strip.Color(0,  127,  0);
uint32_t GREEN  = strip.Color(0,    0,127);
uint32_t YELLOW = strip.Color(127,  0,127);
uint32_t PINK   = strip.Color(127,100,  0);
uint32_t CYAN   = strip.Color(0,  127,127);
uint32_t PURPLE = strip.Color(10,  20,  0);
uint32_t ORANGE = strip.Color(100,  0, 50);
uint32_t WHITE  = strip.Color(127,127,127);

void setup() {
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
  clock_prescale_set(clock_div_1); // Enable 16 MHz on Trinket
#endif

  // Start up the LED strip
  strip.begin();

  // Update the strip, to start they are all 'off'
  strip.show();
}


void loop() {
  solid(RED,   1500);
  solid(ORANGE,1500);
  solid(YELLOW,1500);
  solid(GREEN, 1500);
  solid(BLUE,  1500);
  solid(PURPLE,1500);
  
  cylon(0);

  americanFlag(5000);
  prideFlag(5000);
  
  // Send a simple pixel chase in...
  colorChase(WHITE, 0);
  colorChase(RED,   0);
  colorChase(GREEN, 0);
  colorChase(BLUE,  0);
  colorChase(PURPLE,0);

  // Send a theater pixel chase in...
  theaterChase(WHITE, 50);
  theaterChase(RED,   50);
  theaterChase(GREEN, 50);
  theaterChase(BLUE,  50);
  theaterChase(PURPLE,50);

  // Fill the entire strip with...
  colorWipe(RED,   10);
  colorWipe(GREEN, 10);
  colorWipe(BLUE,  10);

  rainbow(10);
//  theaterChaseRainbow(50);
  
}

void turnAllOff() {
  int i;
  for (i=0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i,0);
  }
  strip.show(); 
}

void solid(uint32_t c, uint32_t wait) {
  int i;
  for (i=0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i,c);
  }
  strip.show();
  delay(wait);  
}

void americanFlag(uint32_t wait) {
  int i;
  for (i=0; i < TopStart; i++) {
    strip.setPixelColor(i,RED);   
  }
  for (i=TopStart; i < RightStart; i++) {
    strip.setPixelColor(i,WHITE);
  }
  for (i=RightStart; i< strip.numPixels(); i++) {
    strip.setPixelColor(i,BLUE);
  }
  strip.show();
  delay(wait);
}

void prideFlag(uint32_t wait) {
  int i;
  for (i=0; i<48; i++) {
    strip.setPixelColor(i,RED);   
  }
  for (i=48; i<96; i++) {
    strip.setPixelColor(i,ORANGE);   
  }
  for (i=96; i< 143; i++) {
    strip.setPixelColor(i,YELLOW);
  }
  for (i=143; i<190; i++) {
    strip.setPixelColor(i,GREEN);
  }
  for (i=190; i<238; i++) {
    strip.setPixelColor(i,BLUE);
  }
  for (i=238; i<286; i++) {
    strip.setPixelColor(i,PURPLE);
  }
  strip.show();
  delay(wait);
}

void indexTest(uint32_t wait) {
  turnAllOff();
  
  strip.setPixelColor(0,RED);
  strip.setPixelColor(95,GREEN);
  strip.setPixelColor(190,BLUE);
  strip.setPixelColor(285,WHITE);
  strip.show();
  delay(30);
}

void cylon(uint8_t wait) {
  int width = 10;
  long color = strip.Color(127,0,0);

  int i,j;
  for (i=0; i < width; i++) {
    strip.setPixelColor(i,color);
  }
  strip.show();

  for (j=0; j<5; j++) {
    for (i=width+1; i < strip.numPixels(); i++) {
      strip.setPixelColor(i,color);
      strip.setPixelColor(i-width-1,0);
      strip.show();
      delay(wait); 
    }
    for (i=strip.numPixels(); i > width; i--) {
      strip.setPixelColor(i,0);
      strip.setPixelColor(i-width-1,color);
      strip.show();
      delay(wait); 
    }
  }
}

void rainbow(uint8_t wait) {
  int i, j;
   
  for (j=0; j < 384; j++) {     // 3 cycles of all 384 colors in the wheel
    for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel( (i + j) % 384));
    }  
    strip.show();   // write all the pixels out
    delay(wait);
  }
}

// Slightly different, this one makes the rainbow wheel equally distributed 
// along the chain
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;
  
  for (j=0; j < 384 * 5; j++) {     // 5 cycles of all 384 colors in the wheel
    for (i=0; i < strip.numPixels(); i++) {
      // tricky math! we use each pixel as a fraction of the full 384-color wheel
      // (thats the i / strip.numPixels() part)
      // Then add in j which makes the colors go around per pixel
      // the % 384 is to make the wheel cycle around
      strip.setPixelColor(i, Wheel( ((i * 384 / strip.numPixels()) + j) % 384) );
    }  
    strip.show();   // write all the pixels out
    delay(wait);
  }
}

// Fill the dots progressively along the strip.
void colorWipe(uint32_t c, uint8_t wait) {
  int i;

  for (i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

// Chase one dot down the full strip.
void colorChase(uint32_t c, uint8_t wait) {
  int i;

  // Start by turning all pixels off:
  for(i=0; i<strip.numPixels(); i++) strip.setPixelColor(i, 0);

  // Then display one pixel at a time:
  for(i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c); // Set new pixel 'on'
    strip.show();              // Refresh LED states
    strip.setPixelColor(i, 0); // Erase pixel, but don't refresh!
    delay(wait);
  }

  strip.show(); // Refresh to turn off last pixel
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();
     
      delay(wait);
     
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 384; j++) {     // cycle all 384 colors in the wheel
    for (int q=0; q < 3; q++) {
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, Wheel( (i+j) % 384));    //turn every third pixel on
        }
        strip.show();
       
        delay(wait);
       
        for (int i=0; i < strip.numPixels(); i=i+3) {
          strip.setPixelColor(i+q, 0);        //turn every third pixel off
        }
    }
  }
}
/* Helper functions */

//Input a value 0 to 384 to get a color value.
//The colours are a transition r - g -b - back to r

uint32_t Wheel(uint16_t WheelPos)
{
  byte r, g, b;
  switch(WheelPos / 128)
  {
    case 0:
      r = 127 - WheelPos % 128;   //Red down
      g = WheelPos % 128;      // Green up
      b = 0;                  //blue off
      break; 
    case 1:
      g = 127 - WheelPos % 128;  //green down
      b = WheelPos % 128;      //blue up
      r = 0;                  //red off
      break; 
    case 2:
      b = 127 - WheelPos % 128;  //blue down 
      r = WheelPos % 128;      //red up
      g = 0;                  //green off
      break; 
  }
  return(strip.Color(r,g,b));
}
