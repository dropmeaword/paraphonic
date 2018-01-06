#include "ledstrip.h"
#include "log.h"

CRGB leds[NUM_LEDS];

const int numLeds = NUM_LEDS;
const int numberOfChannels = numLeds * 3;

// pinout for LED strip
const int pinCLK = D7; //D3;
const int pinSDA = D8; //D4;


// ESPIChipsets CHIPSET,  uint8_t DATA_PIN, uint8_t CLOCK_PIN, EOrder RGB_ORDER > static CLEDController &addLeds(struct CRGB *data, int nLedsOrOffset, int nLedsIfOffset = 0) {


void ledstrip_init() { //(const int ledcount, ESPIChipsets chipset, uint8_t  psda, uint8_t  pclk, EOrder rgbcomp) {
  FastLED.addLeds<APA102, pinSDA, pinCLK, BGR>(leds, NUM_LEDS);
  FastLED.setBrightness(96);
  set_max_power_in_volts_and_milliamps(5, 3000);
  LOG("Initializing LEDs...");
  LOG_NEW_LINE
}

void ledstrip_refresh() {
    FastLED.show();
}

void ledstrip_solid(int r, int g, int b) {
  for (int i = 0 ; i < numLeds ; i++) {
    leds[i] = CRGB(r, g, b);
  }
  show_at_max_brightness_for_power();
}

void ledstrip_test_pattern()
{
  int waiting = 1000;

  LOG(">>> pumping test pattern");
  LOG_NEW_LINE

  for (int i = 0 ; i < numLeds ; i++)
    leds[i] = CRGB(127, 0, 0);

  FastLED.show();
  delay(waiting);

  for (int i = 0 ; i < numLeds ; i++)
    leds[i] = CRGB(0, 127, 0);

  FastLED.show();
  delay(waiting);

  for (int i = 0 ; i < numLeds ; i++)
    leds[i] = CRGB( 0, 0, 127);
  FastLED.show();
  delay(waiting);

  for (int i = 0 ; i < numLeds ; i++)
    leds[i] = CRGB( 0, 0, 0);
  FastLED.show();
}

/*
void fade(uint16_t duration, Color startColor, Color endColor) {

    int16_t redDiff = endColor.getR() - startColor.getR();
    int16_t greenDiff = endColor.getG() - startColor.getG();
    int16_t blueDiff = endColor.getB() - startColor.getB();

    int16_t delay = 20;
    int16_t steps = duration / delay;

    int16_t redValue, greenValue, blueValue;

    for (int16_t i = 0 ; i < steps - 1 ; ++i) {
        redValue = (int16_t)startColor.getR() + (redDiff * i / steps);
        greenValue = (int16_t)startColor.getG() + (greenDiff * i / steps);
        blueValue = (int16_t)startColor.getB() + (blueDiff * i / steps);

        Serial.print(redValue);
        Serial.print("\t");
        Serial.print(greenValue);
        Serial.print("\t");
        Serial.print(blueValue);
        Serial.println("\t");

        led.shine(redValue, greenValue, blueValue);
        waitMs(delay);
    }

    led.shine(endColor);
}
*/

void ledstrip_loop() {
    // EVERY_N_MILLISECONDS(thisdelay) {                           // FastLED based non-blocking delay to update/display the sequence.
    //   twinkle();
    // }
    // show_at_max_brightness_for_power();
}