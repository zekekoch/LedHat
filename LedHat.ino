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

int serialValue = 0;
int lightIndex = 0;

void setup()
{
  FastSPI_LED.setLeds(NUM_LEDS);
  FastSPI_LED.setChipset(CFastSPI_LED::SPI_LPD8806);  
  FastSPI_LED.setPin(PIN);
  
  FastSPI_LED.init();
  FastSPI_LED.start();

  leds = (struct CRGB*)FastSPI_LED.getRGBData(); 
  
  Serial.begin(9600);
  Serial.println("ready");
}

void loop()
{
  accelWater();
}

void findLedLoc()
{
  if (Serial.available())
    {
    char ch = Serial.read();
    if (ch >= '0' && ch <= '9')
    { 
      serialValue = (serialValue * 10) + (ch - '0');
      Serial.print(serialValue);
      Serial.println(" ");
    }
    else if (ch == 10) // newline
    {
      Serial.println(serialValue);
      accelDump();
      lightIndex = serialValue;
      setLight(serialValue);
      serialValue = 0;
    }
  }
}

void accelDump()
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

void accelWater()
{
  int x = analogRead(xPin) - 512;
  int y = analogRead(yPin) - 512;
  int z = analogRead(zPin) - 512;

  // clear the strip
  //memset(leds, 0, NUM_LEDS * 3);
  fadeOld();
  
  y = constrain(y, -45, 45);
  
  if (y > 0)
    setFrontBack(x, 128,0, 90 + (y*3));
  else
    setFrontBack(x, 90 + (y*3),0, 128);
        
  FastSPI_LED.show();

}

void fadeOld()
{
  for(int i = 0; i < NUM_LEDS; i++ ) 
  {
    if (leds[i].r > 2)
      leds[i].r -= 2;
    else
      leds[i].r = 0;
      
    if (leds[i].g > 2)
      leds[i].g -= 2;
    else
      leds[i].g = 0;

    if (leds[i].b > 2)
      leds[i].b -= 2;
    else
      leds[i].b = 0;
  }  
}

int setFrontBack(int x, byte r, byte g, byte b)
{

  if (x > 45)
  {
    setPixel(89,r,g,b);
    setPixel(58,r,g,b);
    setPixel(28,r,g,b);
    setPixel(57,r,g,b);
    setPixel(27,r,g,b);
    setPixel(0,r,g,b);
  }
  else if (x < -45)
  {
    setPixel(74,r,g,b);
    setPixel(73,r,g,b);
    setPixel(42,r,g,b);
    setPixel(41
    
    ,r,g,b);
    setPixel(14,r,g,b);
    setPixel(13,r,g,b);
  }
  else
  {
    x = x + 45; // make the range positive
    setPixel(74 + ((x * 10) /64), r,g,b);
    setPixel(73 - ((x * 10) /64), r,g,b);   
    setPixel(42 - (x /6), r,g,b);
    setPixel(41 + (x /6), r,g,b);   
    setPixel(14 - ((x * 10) /64), r,g,b);
    setPixel(13 + ((x * 10) /64), r,g,b);   
  }
  
}

int bottomRow(int x)
{
    if (x > 45)
  {
      setPixelWhite(89,255);
      setPixelWhite(58,255);
  }
  else if (x < -45)
  {
      setPixelWhite(74,255);
      setPixelWhite(73,255);    
  }
  else
  {
    x = x + 45; // make the range positive
      setPixelWhite(74 + ((x * 10) /64), 255);
      setPixelWhite(73 - ((x * 10) /64), 255);   
  }
}

int middleRow(int x)
{
  if (x > 45)
  {
      setPixelWhite(28,255);
      setPixelWhite(57,255);
  }
  else if (x < -45)
  {
      setPixelWhite(40,255);
      setPixelWhite(41,255);    
  }
  else
  {
    x = x + 45; // make the range positive
     setPixelWhite(42 - (x /6), 255);
     setPixelWhite(41 + (x /6), 255);   
  }
}

int topRow(int x)
{
  if (x > 45)
  {
    setPixelWhite(27,255);
    setPixelWhite(0,255);
  }
  else if (x < -45)
  {
    setPixelWhite(40,255);
    setPixelWhite(41,255);    
  }
  else
  {
    x = x + 45; // make the range positive
   setPixelWhite(14 - ((x * 10) /64), 255);
   setPixelWhite(13 + ((x * 10) /64), 255);   
  }
}

void setPixel(int pixel, byte r, byte g, byte b)
{
  leds[pixel].r = r;
  leds[pixel].g = g;
  leds[pixel].b = b;  
}

void setPixelWhite(int pixel, byte brightness)
{
  setPixel(pixel, brightness, brightness * 10 / 15, brightness);
}

void setAllColor(byte r, byte g, byte b)
{
  for(int i = 0; i < NUM_LEDS; i++ ) 
  {
    leds[i].r = r;
    leds[i].g = g;
    leds[i].b = b;
  }      
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

void setLight(int light) 
{
      memset(leds, 0, NUM_LEDS * 3);
      leds[light].r = 255;
      leds[light].g = 255;
      leds[light].b = 255;
      FastSPI_LED.show();  
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

