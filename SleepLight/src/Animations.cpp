#include "Animations.hpp"

#define BREATHE_DUR   20000
#define BREATHE_MIN      10
#define BREATHE_MAX      30


// Used for maintaining Animation in display
void Animations::HandleDisplay() {
  switch (ani_style) {
    case 1:
      // Breathe brightness levels 20 to 50
      ani_pos = ani_dir + ani_pos;
      if (ani_pos > BREATHE_MAX) {
        ani_pos = BREATHE_MAX;
        ani_dir = -1;
      } else if (ani_pos < BREATHE_MIN) {
        ani_pos = BREATHE_MIN;
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
    case 7:
      Siren();
      break;
    default:
      break;
  }
}

void Animations::set_ChangeLED(int switcheroo) {
  if (ani_style != 7) {
    ani_style = 1;
    ani_refresh = BREATHE_DUR / (BREATHE_MAX - BREATHE_MIN + 1);
    ani_pos = switcheroo;
    ani_dir = 0;
    ChangeLED();
  }
}

void Animations::set_RotatingColors() {
  if (ani_style != 7) {
    ani_style = 2;
    ani_refresh = 1000/4;
    ani_pos = 0;
    ani_dir = 0;
    RotatingColors();
  }
}

void Animations::set_ElevatingColors() {
  if (ani_style != 7) {
    ani_style = 3;
    ani_refresh = 1000/5;
    ani_pos = 0;
    ani_dir = 0;
    ElevatingColors();
  }
}

void Animations::set_FadingColors() {
  //if (ani_style != 7) {
    //ani_style = 4;
    //ani_refresh = 1000/20;
    //ani_pos = 0;
    //ani_dir = 0;
    //FadingColors();
  //}
}

void Animations::set_RandomColors() {
  if (ani_style != 7) {
    ani_style = 5;
    ani_pos = 0;
    ani_dir = 0;
    RandomColors();
  }
}

void Animations::set_LavaLamp() {
  //if (ani_style != 7) {
    //ani_style = 6;
    //ani_refresh = 1000/2;
    //ani_pos = 0;
    //ani_dir = 0;
    //LavaLamp();
  //}
}

void Animations::set_Siren() {
  ani_style = 7;
  ani_refresh = 1000/3;
  ani_pos = 0;
  ani_dir = 0;
  Siren();
}

void Animations::ChangeLED() {
  switch (ani_pos) { 
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

}

void Animations::RandomColors() {
  ani_refresh = random(1000,5001);
  for (int icol = 0; icol < LED_ROT; icol++) {
    int color_index = micros() % 6;
    for (int irow = 0; irow < LED_HGT; irow++) {
      leds[LED_location[irow][icol]] = CRGB(color_rgb[color_index][1],color_rgb[color_index][2],color_rgb[color_index][3]);
    }
  }
  FastLED.show();
}

/*
void LavaLamp() {
  if (lava_lamp_on[0] == -1 && lava_lamp_on[1] == -1 && lava_lamp_on[2] == -1) {
    // Set lava lamp pixels
    for (int idex = 0; idex < random(0,3); idex++) {
      lava_lamp_on[idex] = LED_locations[5][random(0,5)];
    }
    Serial.print("lava_lamp_pix (1,2,3): "); Serial.print(lava_lamp_on[0]); Serial.print(", "); Serial.print(lava_lamp_on[1]); Serial.print(", "); Serial.println(lava_lamp_on[2]);
  }
}
*/

void Animations::Siren() {
  ani_pos++;
  if (ani_pos >= LED_ROT) {
    ani_pos = 0;
  }
  bool siren_color[4] = {0,0,0,0};
  switch (ani_pos) {
    case 0:
      siren_color[0] = 1;
      siren_color[1] = 1;
      break;
    case 1:
      siren_color[1] = 1;
      siren_color[2] = 1;
      break;
    case 2:
      siren_color[2] = 1;
      siren_color[3] = 1;
      break;
    case 3:
      siren_color[3] = 1;
      siren_color[0] = 1;
      break;
  }
  FastLED.clear();
    for (int icol = 0; icol < LED_ROT; icol++) {
      int color_index = -1;
      if (siren_color[icol]) {
        color_index = 0;
      } else {
        color_index = 4;
      }
      for (int irow = 0; irow < LED_HGT; irow++) {
        leds[LED_location[irow][icol]] = CRGB(color_rgb[color_index][0],color_rgb[color_index][1],color_rgb[color_index][2]);
      }
    }
    FastLED.show();
}