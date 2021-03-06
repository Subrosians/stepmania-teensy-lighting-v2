// Christopher McKinzie (subrosian@gmail.com)
#include <Adafruit_NeoPixel.h>

#define NUM_LEDS 492
#define DATA_PIN 32
//CRGB leds[NUM_LEDS];
Adafruit_NeoPixel leds = Adafruit_NeoPixel(NUM_LEDS, DATA_PIN, NEO_GRB + NEO_KHZ800);

unsigned long lastbutton = 0;
int attractmode = 0;

const int pacInputNum = 16;

enum pacInput {
  pacNull,
  pacPad1L,
  pacPad1R,
  pacPad1U,
  pacPad1D,
  pacPad2L,
  pacPad2R,
  pacPad2U,
  pacPad2D,
  pacMarqueeUL,
  pacMarqueeUR,
  pacMarqueeLL,
  pacMarqueeLR,
  pacControl1,
  pacControl2,
  pacSubs,
};

typedef struct {
  byte microInput;
  byte oldState;
  byte onOff;
} pacRef;

pacRef pac[pacInputNum];

const int buttonInputNum = 17;

enum buttonInput {
  buttonNull,
  buttonPad1L,
  buttonPad1R,
  buttonPad1U,
  buttonPad1D,
  buttonPad2L,
  buttonPad2R,
  buttonPad2U,
  buttonPad2D,
  buttonControl1L,
  buttonControl1S,
  buttonControl1R,
  buttonControl2L,
  buttonControl2S,
  buttonControl2R,
  buttonBack,
  buttonConfig,
};

typedef struct {
  byte microInput;
  byte oldState;
  byte usbButton;
} buttonRef;

buttonRef button[buttonInputNum];

const int lightOutputNum = 16;

enum lightOutput {
  lightNull,
  lightPad1L,
  lightPad1R,
  lightPad1U,
  lightPad1D,
  lightPad2L,
  lightPad2R,
  lightPad2U,
  lightPad2D,
  lightMarqueeUL,
  lightMarqueeUR,
  lightMarqueeLL,
  lightMarqueeLR,
  lightControl1,
  lightControl2,
  lightSubs,
};

typedef struct {
  int16_t firstLed;
  int16_t numLeds;
  byte oldState;
  byte onOff;
} lightRef;

lightRef light[lightOutputNum];

const int ledPin = 13;

int randomNumber;

void setup() {
  //FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);

  pac[pacPad1L].microInput = 1;
  pac[pacPad1R].microInput = 2;
  pac[pacPad1U].microInput = 3;
  pac[pacPad1D].microInput = 4;
  pac[pacPad2L].microInput = 5;
  pac[pacPad2R].microInput = 6;
  pac[pacPad2U].microInput = 7;
  pac[pacPad2D].microInput = 8;
  pac[pacMarqueeUL].microInput = 9;
  pac[pacMarqueeUR].microInput = 10;
  pac[pacMarqueeLL].microInput = 11;
  pac[pacMarqueeLR].microInput = 12;
  pac[pacControl1].microInput = 14;
  pac[pacControl2].microInput = 15;
  pac[pacSubs].microInput = 16;

  button[buttonPad1L].microInput = 17;
  button[buttonPad1L].usbButton = 1;
  button[buttonPad1R].microInput = 18;
  button[buttonPad1R].usbButton = 2;
  button[buttonPad1U].microInput = 19;
  button[buttonPad1U].usbButton = 3;
  button[buttonPad1D].microInput = 20;
  button[buttonPad1D].usbButton = 4;
  button[buttonPad2L].microInput = 21;
  button[buttonPad2L].usbButton = 5;
  button[buttonPad2R].microInput = 22;
  button[buttonPad2R].usbButton = 6;
  button[buttonPad2U].microInput = 23;
  button[buttonPad2U].usbButton = 7;
  button[buttonPad2D].microInput = 24;
  button[buttonPad2D].usbButton = 8;
  button[buttonControl1L].microInput = 25;
  button[buttonControl1L].usbButton = 9;
  button[buttonControl1S].microInput = 26;
  button[buttonControl1S].usbButton = 10;
  button[buttonControl1R].microInput = 27;
  button[buttonControl1R].usbButton = 11;
  button[buttonControl2L].microInput = 28;
  button[buttonControl2L].usbButton = 12;
  button[buttonControl2S].microInput = 29;
  button[buttonControl2S].usbButton = 13;
  button[buttonControl2R].microInput = 30;
  button[buttonControl2R].usbButton = 14;
  button[buttonBack].microInput = 33;
  button[buttonBack].usbButton = 15;
  button[buttonConfig].microInput = 34;
  button[buttonConfig].usbButton = 16;



  light[lightMarqueeUL].firstLed = 0;
  light[lightMarqueeUL].numLeds = 12;
  light[lightMarqueeUR].firstLed = 12;
  light[lightMarqueeUR].numLeds = 12;
  light[lightMarqueeLL].firstLed = 24;
  light[lightMarqueeLL].numLeds = 12;
  light[lightMarqueeLR].firstLed = 36;
  light[lightMarqueeLR].numLeds = 12;

  light[pacControl1].firstLed = 350;
  light[pacControl1].numLeds = 1;
  light[pacControl2].firstLed = 351;
  light[pacControl2].numLeds = 1;
  light[pacSubs].firstLed = 54;
  light[pacSubs].numLeds = 104;

  light[lightPad1U].firstLed = 158;
  light[lightPad1U].numLeds = 24;
  light[lightPad1D].firstLed = 182;
  light[lightPad1D].numLeds = 24;
  light[lightPad1L].firstLed = 206;
  light[lightPad1L].numLeds = 24;
  light[lightPad1R].firstLed = 230;
  light[lightPad1R].numLeds = 24;


  light[lightPad2U].firstLed = 254;
  light[lightPad2U].numLeds = 24;
  light[lightPad2D].firstLed = 278;
  light[lightPad2D].numLeds = 24;
  light[lightPad2L].firstLed = 302;
  light[lightPad2L].numLeds = 24;
  light[lightPad2R].firstLed = 326;
  light[lightPad2R].numLeds = 24;


  Serial.begin(9600);
  Serial.println("INFO: Microcontroller Initialized");
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);

  //setup all pacinputs as inputs
  for (int counter = 0; counter < pacInputNum; counter++) {
    pinMode(pac[counter].microInput, INPUT_PULLUP);
  }

  //setup all the buttons as inputs
  for (int counter = 0; counter < buttonInputNum; counter++) {
    pinMode(button[counter].microInput, INPUT_PULLUP);
  }

  leds.begin();
}

int reDraw=0;

void fadeLeds(){
  int x=0;
  uint8_t r,g,b;
  
  for(int x=0;x<NUM_LEDS;x++){

    r=(leds.getPixelColor(x) >> 16);
    g=(leds.getPixelColor(x) >> 8);
    b=(leds.getPixelColor(x));
    
    if(r > 0){
      r = int(r *0.90);
    }
    if(g> 0){
      g = int(g *0.90);
    }
    if(b > 0){
      b = int(b *0.90);
    }

    leds.setPixelColor(x,r,g,b);
    
  }
}

void HSV_to_RGB(float h, float s, float v, byte *r, byte *g, byte *b)
{
  int i;
  float f,p,q,t;
  
  h = max(0.0, min(360.0, h));
  s = max(0.0, min(100.0, s));
  v = max(0.0, min(100.0, v));
  
  s /= 100;
  v /= 100;
  
  if(s == 0) {
    // Achromatic (grey)
    *r = *g = *b = round(v*255);
    return;
  }

  h /= 60; // sector 0 to 5
  i = floor(h);
  f = h - i; // factorial part of h
  p = v * (1 - s);
  q = v * (1 - s * f);
  t = v * (1 - s * (1 - f));
  switch(i) {
    case 0:
      *r = round(255*v);
      *g = round(255*t);
      *b = round(255*p);
      break;
    case 1:
      *r = round(255*q);
      *g = round(255*v);
      *b = round(255*p);
      break;
    case 2:
      *r = round(255*p);
      *g = round(255*v);
      *b = round(255*t);
      break;
    case 3:
      *r = round(255*p);
      *g = round(255*q);
      *b = round(255*v);
      break;
    case 4:
      *r = round(255*t);
      *g = round(255*p);
      *b = round(255*v);
      break;
    default: // case 5:
      *r = round(255*v);
      *g = round(255*p);
      *b = round(255*q);
    }
}

void loop() {
  //reset redraw value and check for changes
  reDraw=0;
  
  //check all the pacinputs
  for (int counter = 0; counter < pacInputNum; counter++) {
    //if oldstate does NOT !equal new readstate do something
    int temp = digitalRead(pac[counter].microInput);
    if (pac[counter].oldState != temp) {
      //change in input update leds
      reDraw=1;
      
      //save new old value
      pac[counter].oldState = temp;
      if (temp == 0) {
        pac[counter].onOff = 1;
        //randomNumber = random8();
        uint8_t r,g,b;
        HSV_to_RGB((random(360)/360.0)*360.0,100.0f,50.0f,&r,&g,&b);
        
        for (int16_t ledCounter = light[counter].firstLed; ledCounter < light[counter].firstLed + light[counter].numLeds; ledCounter++) {
          //leds[ledCounter] = CHSV(randomNumber, 255, 255);
          leds.setPixelColor(ledCounter,r,g,b);
        }
      }
      if (temp == 1) {
        pac[counter].onOff = 0;
        for (int16_t ledCounter = light[counter].firstLed; ledCounter < light[counter].firstLed + light[counter].numLeds; ledCounter++) {
          //leds[ledCounter] = CRGB::Black;
          leds.setPixelColor(ledCounter,0, 0, 0);
        }
      }
    }
  }
  
  //needs a redraw
  fadeLeds();
  
  if(reDraw==1 or 1){
    //FastLED.show();
    leds.show();
    reDraw=0;
  }
  
  //check all the buttons
  for (int counter = 1; counter < buttonInputNum; counter++) {
    //if oldstate does NOT !equal new readstate do something
    if (button[counter].oldState != digitalRead(button[counter].microInput)) {
      button[counter].oldState = digitalRead(button[counter].microInput);
      //send button state to joystick using oposite new oldstate since !(HIGH=1)=0 and !(LOW=0)=1
      Joystick.button(button[counter].usbButton, !button[counter].oldState);

    }
  }
}
