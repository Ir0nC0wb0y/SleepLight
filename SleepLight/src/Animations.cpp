#include "Animations.hpp"

void Animations::HandleDisplay() {
  switch (ani_style) {
    case 1:
      // Breathe brightness levels 20 to 50
      ani_pos = ani_dir + ani_pos;
      if (ani_pos > 50) {
        ani_pos = 50;
        ani_dir = -1;
      } else if (ani_pos < 20) {
        ani_pos = 20;
        ani_dir = 1;
      }
      FastLED.setBrightness(ani_pos);
      FastLED.show();
      break;
    case 2:
      RotatingColors();
      break;
    case 3:
      ElevatingColors();
      break;
    case 4:
      ani_style = 0;
      //FadingColors();
      break;
    case 5:
      RandomColors();
      break;
    case 6:
      ani_style = 0;
      //LavaLamp();
      break;
    default:
      break;
  }
}

void Animations::ChangeLED(int switcheroo) {
  ani_style = 1;
  ani_refresh = 1000/10;
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

void Animations::RotatingColors() {
  ani_style = 2;
  ani_refresh = 1000/4;
  ani_pos++;
  if (ani_pos >= LED_ROT) {
    ani_pos = 0;
  }
    for (int idex = 0; idex < NUM_LEDS; idex++) {
        if (floor(idex/LED_HGT) == ani_pos) {
            leds[idex] = CRGB::Red;
        } else {
            leds[idex] = CRGB::Green;
        }
    }
    FastLED.show();
}

void Animations::ElevatingColors() {
  ani_style = 3;
  ani_refresh = 1000/5;
    for (int idex = 0; idex < NUM_LEDS; idex++) {
        if (idex % LED_ROT == ani_pos) {
            leds[idex] = CRGB::Chartreuse;
        } else {
            leds[idex] = CRGB::Blue;
        }
    }
    FastLED.show();
}

void Animations::FadingColors() {
  ani_style = 4;
  ani_refresh = 1000/20;

}

void Animations::RandomColors() {
  ani_style = 5;
  ani_refresh = 1000/5;
  for (int icol = 0; icol < LED_ROT; icol++) {
    int color_index = random(0,5);
    for (int irow = 0; irow < LED_HGT; irow++) {
      leds[LED_location[irow][icol]] = CRGB(color_rgb[color_index][1],color_rgb[color_index][2],color_rgb[color_index][3]);
    }
  }
  FastLED.show();
}

/*
void LavaLamp() {
  ani_style = 6;
  ani_refresh = 1000/2;
  if (lava_lamp_on[0] == -1 && lava_lamp_on[1] == -1 && lava_lamp_on[2] == -1) {
    // Set lava lamp pixels
    for (int idex = 0; idex < random(0,3); idex++) {
      lava_lamp_on[idex] = LED_locations[5][random(0,5)];
    }
    Serial.print("lava_lamp_pix (1,2,3): "); Serial.print(lava_lamp_on[0]); Serial.print(", "); Serial.print(lava_lamp_on[1]); Serial.print(", "); Serial.println(lava_lamp_on[2]);
  }
}
*/