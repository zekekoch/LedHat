#include <FastSPI_LED.h>

#define NUM_LEDS 90

// Sometimes chipsets wire in a backwards sort of way
struct CRGB { unsigned char b; unsigned char r; unsigned char g; };
// struct CRGB { unsigned char r; unsigned char g; unsigned char b; };
struct CRGB *leds;

#define PIN 13

int xPin = A1;
int yPin = A2;
int zPin = A3;

void setup()
{
  FastSPI_LED.setLeds(NUM_LEDS);
  //FastSPI_LED.setChipset(CFastSPI_LED::SPI_SM16716);
  //FastSPI_LED.setChipset(CFastSPI_LED::SPI_TM1809);
  FastSPI_LED.setChipset(CFastSPI_LED::SPI_LPD8806);  
  //FastSPI_LED.setChipset(CFastSPI_LED::SPI_LPD6803);
  //FastSPI_LED.setChipset(CFastSPI_LED::SPI_HL1606);
  //FastSPI_LED.setChipset(CFastSPI_LED::SPI_595);
  //FastSPI_LED.setChipset(CFastSPI_LED::SPI_WS2801);

  FastSPI_LED.setPin(PIN);
  
  FastSPI_LED.init();
  FastSPI_LED.start();

  leds = (struct CRGB*)FastSPI_LED.getRGBData(); 
}

void loop()
{
  accel();
}

void loop3()
{
  Serial.print("x=");
  Serial.print(analogRead(xPin)-512);
  Serial.print("y=");
  Serial.print(analogRead(yPin)-512);
  Serial.print("z=");
  Serial.print(analogRead(zPin)-512);
  Serial.println();
  
  //dim();
 
}

void accel()
{
  int x = analogRead(xPin) - 512;
  int y = analogRead(yPin) - 512;
  int z = analogRead(zPin) - 512;
  
  memset(leds, 0, NUM_LEDS * 3);
  for(int i = 0; i < NUM_LEDS; i++ ) 
  {
    
    if (y > 0)
    {
      leds[i].r = y/2;
      leds[i].b = 0;
    }
    else
    {
      leds[i].r = 0;
      leds[i].b = y/2;
    }

    if (x < 0)
    {
      leds[i].r += -x/2;
    }
          
    if (z > 0)
      leds[i].g = z/2;
    else 
      leds[i].g = 0;
  }
  FastSPI_LED.show();
}

void fade() 
{
   for(int j = 0; j < 3; j++ ) 
   { 
    memset(leds, 0, NUM_LEDS * 3);
    for(int k = 0; k < 256; k++) 
    { 
      for(int i = 0; i < NUM_LEDS; i++ ) 
      {
        switch(j) 
        { 
          case 0: leds[i].r = k; break;
          case 1: leds[i].g = k; break;
          case 2: leds[i].b = k; break;
        }
      }
      FastSPI_LED.show();
      delay(3);
    }
    for(int k = 255; k >= 0; k--) 
    { 
      for(int i = 0; i < NUM_LEDS; i++ ) 
      {
        switch(j) 
        { 
          case 0: leds[i].r = k; break;
          case 1: leds[i].g = k; break;
          case 2: leds[i].b = k; break;
        }
      }
      FastSPI_LED.show();
      delay(3);
    }
  }
}

void oneAtATime() { 
  // one at a time
  for(int j = 0; j < 3; j++) { 
    for(int i = 0 ; i < NUM_LEDS; i++ ) {
      memset(leds, 0, NUM_LEDS * 3);
      switch(j) { 
        case 0: leds[i].r = 255; break;
        case 1: leds[i].g = 255; break;
        case 2: leds[i].b = 255; break;
      }
      FastSPI_LED.show();
      delay(10);
    }
  }
}

void grow() {
  // growing/receeding bars
  for(int j = 0; j < 3; j++) { 
    memset(leds, 0, NUM_LEDS * 3);
    for(int i = 0 ; i < NUM_LEDS; i++ ) {
      switch(j) { 
        case 0: leds[i].r = 255; break;
        case 1: leds[i].g = 255; break;
        case 2: leds[i].b = 255; break;
      }
      FastSPI_LED.show();
      delay(10);
    }
    for(int i = NUM_LEDS-1 ; i >= 0; i-- ) {
      switch(j) { 
        case 0: leds[i].r = 0; break;
        case 1: leds[i].g = 0; break;
        case 2: leds[i].b = 0; break;
      }
      FastSPI_LED.show();
      delay(1);
    }
  }
  
  // Fade in/fade out
  for(int j = 0; j < 3; j++ ) { 
    memset(leds, 0, NUM_LEDS * 3);
    for(int k = 0; k < 256; k++) { 
      for(int i = 0; i < NUM_LEDS; i++ ) {
        switch(j) { 
          case 0: leds[i].r = k; break;
          case 1: leds[i].g = k; break;
          case 2: leds[i].b = k; break;
        }
      }
      FastSPI_LED.show();
      delay(3);
    }
    for(int k = 255; k >= 0; k--) { 
      for(int i = 0; i < NUM_LEDS; i++ ) {
        switch(j) { 
          case 0: leds[i].r = k; break;
          case 1: leds[i].g = k; break;
          case 2: leds[i].b = k; break;
        }
      }
      FastSPI_LED.show();
      delay(3);
    }
  }
}

