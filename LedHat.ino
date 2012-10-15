  #include <FastSPI_LED.h>
  
  #define NUM_LEDS 90
  #define PIN 13
  #define xPin A1
  #define yPin A2
  #define zPin A3

  // Sometimes chipsets wire in a backwards sort of way
  struct CRGB { unsigned char b; unsigned char r; unsigned char g; };
  // struct CRGB { unsigned char r; unsigned char g; unsigned char b; };
  struct CRGB *leds;
  
  struct LedStrip {
    byte first;
    byte middle;
    byte last;
    byte length;
    byte dir;
    byte stepSize;
  };
  
  LedStrip strips[3] = 
    {
      {89,74, 58,31,1, 58},
      {57,42, 28,29,-1, 62},
      {27,14, 0,27,1, 67}
    };
  
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
        accelDump(analogRead(xPin) - 512, analogRead(yPin) - 512, analogRead(zPin) - 512);
        lightIndex = serialValue;
        setLight(serialValue);
        serialValue = 0;
      }
    }
  }
  
  void accelDump(int x, int y, int z)
  {
    Serial.print("x=");
    Serial.print(x);
    Serial.print(" y=");
    Serial.print(y);
    Serial.print(" z=");
    Serial.print(z);
    Serial.println();
    delay(200);
  
  }
  
  void accelWater()
  {
    // the accelerometer reads about 512 on each axis when the hat is level
    int x = analogRead(xPin) - 512;
    int y = analogRead(yPin) - 512;
    int z = analogRead(zPin) - 512;
    //accelDump(x, y, z);
  
    // clear the strip
    //memset(leds, 0, NUM_LEDS * 3);
  
    // slowly fade out the old LEDs giving a nice ghosting effect
    fadeOld();
    
    // temp for debugging
    //memset(leds, 0, NUM_LEDS * 3);
    
    
    y = constrain(y, -63, 63);
    y = y * 2;
    
    // red on the top, purple in the middle and blue on the bottom
    if (y > 0)
    {
      setFrontBack(x, 128 - y,0, 128 + y);
    }
    else
    {
      setFrontBack(x, 128 -y,0, 128+y);
    }
    
    FastSPI_LED.show();
  }
  
  void fadeOld()
  {
    byte fadeDur = 1;
    for(int i = 0; i < NUM_LEDS; i++ ) 
    {
      if (leds[i].r > fadeDur)
        leds[i].r -= fadeDur;
      else
        leds[i].r = 0;
        
      if (leds[i].g > fadeDur)
        leds[i].g -= fadeDur;
      else
        leds[i].g = 0;
  
      if (leds[i].b > fadeDur)
        leds[i].b -= fadeDur;
      else
        leds[i].b = 0;
    }  
  }
  
  int setFrontBack(int x, byte r, byte g, byte b)
  {
  
      x = constrain(x, -45, 45);
      for (int i = 0;i<3;i++)
      {
        //Serial.print("row: ");Serial.print(i);Serial.print(" ");Serial.print(x);Serial.println();
        int currentStep = ((x+45)*10)/(strips[i].stepSize);
        
        if (strips[i].dir == -1)
        {
          //Serial.print("-> currentStep ");Serial.println(currentStep);
          setPixel(strips[i].first + currentStep,r,128,b);
          setPixel(strips[i].last - currentStep,r,128,b);
        }
        else
        {
          //Serial.print("<- currentStep ");Serial.println(currentStep);
          setPixel(strips[i].first + currentStep,r,255,b);
          setPixel(strips[i].last - currentStep,r,255,b);
        }
     }
  }
  
  void setPixel(int pixel, byte r, byte g, byte b)
  {
    leds[pixel].r = r;
    leds[pixel].g = g;
    leds[pixel].b = b;  
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

