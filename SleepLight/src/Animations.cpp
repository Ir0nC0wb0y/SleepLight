#include <FastLED.h>

#include "Common.h"

void RotatingColors(int rot_pos) {
    for (int idex = 0; idex < NUM_LEDS; idex++) {
        if (floor(idex/LED_HGT) == rot_pos) {
            leds[idex] = CRGB::White;
        } else {
            leds[idex] = CRGB::Chartreuse;
        }
    }
    FastLED.show();
}

void ElevatingColors(int hgt_pos) {
    for (int idex = 0; idex < NUM_LEDS; idex++) {
        if (idex % LED_ROT == hgt_pos) {
            leds[idex] = CRGB::Chartreuse;
        } else {
            leds[idex] = CRGB::Blue;
        }
    }
    FastLED.show();
}