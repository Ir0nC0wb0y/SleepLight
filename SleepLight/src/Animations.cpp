#include "Animations.hpp"

void Animations::ChangeLED(int switcheroo) {
  switch (switcheroo) { 
    case 1:
      // Green
      for(int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed++) {
        // Turn our current led on to white, then show the leds
        leds[whiteLed] = CRGB::Green;
      }
      //Serial.println("Changing LEDS to Green");
      break;
    case 2:
      // Blue
      for(int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed++) {
        // Turn our current led on to white, then show the leds
        leds[whiteLed] = CRGB::Blue;
      }
      //Serial.println("Changing LEDS to Blue");
      break;
    case 3:
      // Purple
      for(int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed++) {
        // Turn our current led on to white, then show the leds
        leds[whiteLed] = CRGB::Purple;
      }
      //Serial.println("Changing LEDS to Purple");
      break;
    case 4:
      // White
      for(int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed++) {
        // Turn our current led on to white, then show the leds
        leds[whiteLed] = CRGB::White;
      }
      //Serial.println("Changing LEDS to White");
      break;

  }
   FastLED.show();
}

void Animations::RotatingColors(int rot_pos) {
    for (int idex = 0; idex < NUM_LEDS; idex++) {
        if (floor(idex/LED_HGT) == rot_pos) {
            leds[idex] = CRGB::White;
        } else {
            leds[idex] = CRGB::Chartreuse;
        }
    }
    FastLED.show();
}

void Animations::ElevatingColors(int hgt_pos) {
    for (int idex = 0; idex < NUM_LEDS; idex++) {
        if (idex % LED_ROT == hgt_pos) {
            leds[idex] = CRGB::Chartreuse;
        } else {
            leds[idex] = CRGB::Blue;
        }
    }
    FastLED.show();
}

void Animations::FadingColors() {

}