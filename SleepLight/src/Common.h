#include <FastLED.h>

#define LED_HGT                     5
#define LED_ROT                     4
#define NUM_LEDS      LED_HGT*LED_ROT
extern CRGB leds[NUM_LEDS];

extern int sched_wake[7][3];
extern int sched_sleep[7][3];