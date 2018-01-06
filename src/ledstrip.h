#ifndef __LEDSTRIP_H__
#define __LEDSTRIP_H__

#include <FastLED.h>

// LED settings
#define STRIPS 7
#define LEDS_PER_STRIP 28
#define NUM_LEDS LEDS_PER_STRIP * STRIPS

extern const int numLeds;
extern const int numberOfChannels;

extern CRGB leds[NUM_LEDS];

extern const int pinSDA;
extern const int pinCLK;

void ledstrip_init();
void ledstrip_refresh();
void ledstrip_test_pattern();

void ledstrip_solid(int r, int g, int b);
void ledstrip_loop();

#endif // __LEDSTRIP_H__