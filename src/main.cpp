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

void setup() {
  delay(3000); // 3 second delay for recovery

  //FastLED.addLeds<APA102>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<APA102>(leds, NUM_LEDS);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
  pinMode(7, OUTPUT);
  digitalWrite(7, HIGH);  // enable access to LEDs
}

uint8_t gHue = 0; // rotating "base color" used by many of the patterns

void loop()
{

  my_fill_rainbow( leds_1, NUM_LEDS_PER_STRIP, gHue, 4);
  my_fill_rainbow( leds_2, NUM_LEDS_PER_STRIP, gHue, 4,1);
  my_fill_rainbow( leds_3, NUM_LEDS_PER_STRIP, gHue, 4);
  my_fill_rainbow( leds_4, NUM_LEDS_PER_STRIP, gHue, 4,1);
  my_fill_rainbow( leds_5, NUM_LEDS_PER_STRIP, gHue, 4);
  my_fill_rainbow( leds_6, NUM_LEDS_PER_STRIP, gHue, 4,1);
  my_fill_rainbow( leds_7, NUM_LEDS_PER_STRIP, gHue, 4);
  my_fill_rainbow( leds_8, NUM_LEDS_PER_STRIP, gHue, 4,1);

  // send the 'leds' array out to the actual LED strip
  FastLED.show();
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND);

  // do some periodic updates
  EVERY_N_MILLISECONDS( 1 ) { gHue = gHue+1; } // slowly cycle the "base color" through the rainbow
}
