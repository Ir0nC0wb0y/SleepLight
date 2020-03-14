#include "Animations.hpp"

#define BREATHE_DUR   20000
#define BREATHE_MIN       5
#define BREATHE_MAX      55


// Used for maintaining Animation in display
void Animations::HandleDisplay() {
  switch (ani_style) {
    case 1:
      // Breathe between Min and Max brightness levels
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
      LavaLamp();
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
  if (ani_style != 7) {
    ani_style = 6;
    ani_refresh = 3000;
    ll_on = 0;
    LL_reset();
    LavaLamp();
  }
}

void Animations::set_Siren() {
  ani_style = 7;
  ani_refresh = 1000/3;
  ani_pos = 0;
  ani_dir = 0;
  Siren();
}

void Animations::ChangeLED() {
  /*switch (ani_pos) { 
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
  */
  for(int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed++) {
        // Turn our current led on to white, then show the leds
        leds[whiteLed] = CRGB(color_rgb[ani_pos][0],color_rgb[ani_pos][1],color_rgb[ani_pos][2]);
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
  Serial.print("Colors:");
  for (int icol = 0; icol < LED_ROT; icol++) {
    int color_index = micros() % 6;
    for (int irow = 0; irow < LED_HGT; irow++) {
      leds[LED_location[irow][icol]] = CRGB(color_rgb[color_index][1],color_rgb[color_index][2],color_rgb[color_index][3]);
    }
    Serial.print(" "); Serial.print(icol); Serial.print(","); Serial.print(color_index);
  }
  Serial.println();
  FastLED.show();
}


void Animations::LavaLamp() {
  // Check that there are LED's on
  if (ll_on == 0) {
    ll_indexes[0][0] = 4;
    ll_indexes[0][1] = random(0,4);
    ll_on = 1;
    Serial.println("turned on LED");
  }
  LL_show();

  // Move the LEDs that are on
  LL_move();
  LL_show();

  // Add LED?
  if (ll_on < 3) {
    LL_add();
    LL_show();
  }

  // Set LEDs
  LL_set();
}

void Animations::LL_reset() {
  for (int irow = 0; irow < 3; irow++) {
    for (int icol = 0; icol < 2; icol++) {
      ll_locations[irow] = -1;
    }
  }
}

void Animations::LL_move() {
  Serial.println("Moving LEDs");
  Serial.print("ll_on: "); Serial.println(ll_on);
  for (int idex = 0; idex < ll_on; idex++) {
    ll_indexes[idex][0]--;
    if (ll_indexes[idex][0] < 0) {
      Serial.print("LED Height: "); Serial.println(ll_indexes[idex][0]);
      // move the remaining ll_indexes up
      for (int idex_remove = idex + 1; idex_remove < ll_on; idex_remove++ ) {
        ll_indexes[idex_remove-1][0] = ll_indexes[idex_remove][0];
        ll_indexes[idex_remove-1][1] = ll_indexes[idex_remove][1];
      }
      ll_on--;
      Serial.println("Removed LED");
      continue;
    }
    ll_indexes[idex][1] = ll_indexes[idex][1] + random(-1,2);
    // validate the rotation of the LED's
    if (ll_indexes[idex][1] >= LED_ROT) {
      ll_indexes[idex][1] = 0;
    } else if (ll_indexes[idex][1] < 0) {
      ll_indexes[idex][1] = 4;
    }
  }
}

void Animations::LL_add() {
  if (round((double) random(0,101)/100.0) == 1) {
    ll_on++;
    ll_indexes[ll_on][0] = 4;
    // Check to see if there is a pixel in the bottom 2 rows, if so, choose a column away from the existing's possible movement.
    int ll_check = true;
    int ll_new = 10;
    for (int idex = 0; idex < ll_on; idex++) {
      int cur_idex = ll_indexes[idex][1];
      if(cur_idex > 2) {
        ll_check = false;
        // take care of +0
        ll_new = ll_new - (cur_idex + 1);
        // Take care of +1
        if (cur_idex + 1 >= LED_ROT) {
          ll_new = ll_new - 1;
        } else {
          ll_new = ll_new - (cur_idex + 1);
        }
        // Take care of -1
        if (cur_idex - 1 < 0) {
          ll_new = ll_new - 4;
        } else {
          ll_new = ll_new - (cur_idex - 1);
        }
      }
    }
    if (ll_check) {
      ll_indexes[ll_on][1] = random(0,4);
    } else if (ll_new > 0) {
      ll_indexes[ll_on][1] = ll_new;
    }
  }
}

void Animations::LL_set() {
  Serial.println("Setting LEDs");
  // Set ll_locations for all "on"
  for (int idex = 0; idex < ll_on; idex++) {
    ll_locations[idex] = LED_location[ll_indexes[idex][0]][ll_indexes[idex][1]];
  }

  for (int set_row = 0; set_row < LED_HGT; set_row++) {
    for (int set_col = 0; set_col < LED_ROT; set_col++) {
      bool cur_found = false;
      for (int finder = 0; finder < ll_on; finder++) {
        if (ll_locations[finder] == LED_location[set_row][set_col]) {
          cur_found = true;
        }
      }
      if (cur_found) {
        Serial.print(LED_location[set_row][set_col]); Serial.print(", "); Serial.print("Blue");
        leds[LED_location[set_row][set_col]] = CRGB::Blue;
      } else {
        leds[LED_location[set_row][set_col]] = CRGB::Black;
        Serial.print(LED_location[set_row][set_col]); Serial.print(", "); Serial.print("Gray");
      }
      Serial.print(" ");
    }
    Serial.println();
  }
  FastLED.show();
}

void Animations::LL_show() {
  Serial.print("LEDs on: "); Serial.println(ll_on);
  for (int idex = 0; idex < 3; idex++) {
    Serial.print("ll_indexes (row, col): "); Serial.print(ll_indexes[idex][0]); Serial.print(", ");Serial.println(ll_indexes[idex][1]);
  }
}

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