#include "FastLED.h"

FASTLED_USING_NAMESPACE

#define NUM_LEDS_PER_STRIP 72

#define NUM_LEDS NUM_LEDS_PER_STRIP*8

CRGBArray<NUM_LEDS> leds;

CRGBSet leds_1(leds, 0,                                                                    NUM_LEDS_PER_STRIP);
CRGBSet leds_2(leds, NUM_LEDS_PER_STRIP,                                                   NUM_LEDS_PER_STRIP);
CRGBSet leds_3(leds, NUM_LEDS_PER_STRIP*2,                                                 NUM_LEDS_PER_STRIP);
CRGBSet leds_4(leds, NUM_LEDS_PER_STRIP*3,                                                 NUM_LEDS_PER_STRIP);
CRGBSet leds_5(leds, NUM_LEDS_PER_STRIP*4,                                                 NUM_LEDS_PER_STRIP);
CRGBSet leds_6(leds, NUM_LEDS_PER_STRIP*5,                                                 NUM_LEDS_PER_STRIP);
CRGBSet leds_7(leds, NUM_LEDS_PER_STRIP*6,                                                 NUM_LEDS_PER_STRIP);
CRGBSet leds_8(leds, NUM_LEDS_PER_STRIP*7,                                                 NUM_LEDS_PER_STRIP);

#define BRIGHTNESS          3
#define FRAMES_PER_SECOND  120

//Forward Declare Functions
void nextPattern();
void rainbow();
void rainbowWithGlitter();
void addGlitter( fract8 chanceOfGlitter);
void myconfetti(struct CRGB * pFirstLED);
void confetti();
void rainbowrain();

uint8_t gHue = 0; // rotating "base color" used by many of the patterns

//period between trail movement for rainbow_rain, milliseconds - for framerate locking
unsigned long p = 14;

bool spinning = false;

void my_fill_rainbow( struct CRGB * pFirstLED, int numToFill,
                  uint8_t initialhue,
                  uint8_t deltahue,
                bool dir=0 )
{
    CHSV hsv;
    hsv.hue = initialhue;
    hsv.val = 255;
    hsv.sat = 240;
    if(dir==1){
      for( int i = 0; i < numToFill; i++) {
          pFirstLED[i] = hsv;
          hsv.hue += deltahue;
      }
    }else{
      for( int i = numToFill-1; i >= 0; i--) {
          pFirstLED[i] = hsv;
          hsv.hue += deltahue;
      }
    }
}

class RainbowRain
{
    //class members ini at startup of class
    unsigned long previous_millis = millis();
    uint8_t rainpos;

    struct CRGB * pleds; 
    uint8_t num_per_strip;

    //constructor
  public:
    RainbowRain(struct CRGB * ipleds, uint8_t inum_per_strip) {
      pleds=ipleds;
      num_per_strip=inum_per_strip;
    }

    void rain(bool dir=0) {
      if (millis() - previous_millis >= p) {
        previous_millis = millis();
        if (dir == 0){
          rainpos++;
          if (rainpos >= num_per_strip) {
            rainpos = 0;
          }
        }
        else{
          rainpos--;
          if (rainpos >= num_per_strip) { //rainpos <= 0 # when rainpos goes -1 it wraps, not checking this lets us address the 0th led
            rainpos = num_per_strip-1;
          }
        }
      pleds[rainpos] += CHSV( gHue, 255, 192);  
      }
    }
};

//Instantiate rainbow rain for each strip
RainbowRain rain1(leds_1, NUM_LEDS_PER_STRIP);
RainbowRain rain2(leds_2, NUM_LEDS_PER_STRIP);
RainbowRain rain3(leds_3, NUM_LEDS_PER_STRIP);
RainbowRain rain4(leds_4, NUM_LEDS_PER_STRIP);
RainbowRain rain5(leds_5, NUM_LEDS_PER_STRIP);
RainbowRain rain6(leds_6, NUM_LEDS_PER_STRIP);
RainbowRain rain7(leds_7, NUM_LEDS_PER_STRIP);
RainbowRain rain8(leds_8, NUM_LEDS_PER_STRIP);


void setup() {
  delay(200); // 3 second delay for recovery

  //FastLED.addLeds<APA102>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<APA102, 11, 13, BGR, DATA_RATE_MHZ(12)>(leds, NUM_LEDS);
  



  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
  pinMode(7, OUTPUT);
  digitalWrite(7, HIGH);  // enable access to LEDs


}
// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
//SimplePatternList gPatterns = { rainbow, confetti, sinelon, juggle, bpm };
SimplePatternList gPatterns = {rainbow};

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current


void loop()
{
  if (spinning){}
  else{
    // Call the current pattern function once, updating the 'leds' array
    gPatterns[gCurrentPatternNumber]();

    // send the 'leds' array out to the actual LED strip
    FastLED.show();
    // insert a delay to keep the framerate modest
    FastLED.delay(1000/FRAMES_PER_SECOND);

    // do some periodic updates
    EVERY_N_MILLISECONDS( 1) { gHue = gHue+1; } // slowly cycle the "base color" through the rainbow
    EVERY_N_SECONDS( 60 ) { nextPattern(); } // change patterns periodically
  }
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

void rainbow()
{
  my_fill_rainbow( leds_1, NUM_LEDS_PER_STRIP, gHue, 4);
  my_fill_rainbow( leds_2, NUM_LEDS_PER_STRIP, gHue, 4,1);
  my_fill_rainbow( leds_3, NUM_LEDS_PER_STRIP, gHue, 4);
  my_fill_rainbow( leds_4, NUM_LEDS_PER_STRIP, gHue, 4,1);
  my_fill_rainbow( leds_5, NUM_LEDS_PER_STRIP, gHue, 4);
  my_fill_rainbow( leds_6, NUM_LEDS_PER_STRIP, gHue, 4,1);
  my_fill_rainbow( leds_7, NUM_LEDS_PER_STRIP, gHue, 4);
  my_fill_rainbow( leds_8, NUM_LEDS_PER_STRIP, gHue, 4,1);
}

void confetti()
{
  fadeToBlackBy( leds, NUM_LEDS, 10);
int pos = random16(NUM_LEDS);
leds[pos] += CHSV( gHue + random8(64), 200, 255);

}

void rainbowrain()
{
  fadeToBlackBy( leds, NUM_LEDS, 17);
  rain1.rain();
  rain2.rain();
  rain3.rain();
  rain4.rain();
  rain5.rain();
  rain6.rain();
  rain7.rain();
  rain8.rain();
}
