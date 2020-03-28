#include "Animations.hpp"

// Constant color LED stuff
#define BREATHE_DUR   10000
#define BREATHE_MIN      20
#define BREATHE_MAX      40

int breath_amp = (BREATHE_MAX - BREATHE_MIN) / 2;
double breath_per = 2 * PI / BREATHE_DUR;
int breath_off = BREATHE_MIN + breath_amp;

// Lava Lamp stuff
#define LL_COLOR_ON_R            0
#define LL_COLOR_ON_G          255
#define LL_COLOR_ON_B            0
#define LL_COLOR_CHANGE_R        0
#define LL_COLOR_CHANGE_G        6
#define LL_COLOR_CHANGE_B        0


// Used for maintaining Animation in display
void Animations::HandleDisplay() {
  int breath_time = millis() - ani_time;
  switch (ani_style) {
    case 1:
      //Serial.print("Cur_time: "); Serial.print(breath_time);
      ani_pos = round((double)breath_amp * sin(breath_per * (double)breath_time) + (double)breath_off);
      //Serial.print(", brightness: "); Serial.println(ani_pos);
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
    ani_time    = millis();
    ani_style   = 1;
    ani_refresh = 1000/20;
    ani_pos     = switcheroo;
    ani_dir     = 0;
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
    FastLED.setBrightness(100);
    RandomColors();
  }
}

void Animations::set_LavaLamp() {
  if (ani_style != 7) {
    ani_style = 6;
    ani_refresh = 50;
    ani_dir = 1;
    ll_on = 0;
    FastLED.setBrightness(192);
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
  // Start 1 pixel if necessary
  if (ll_on < 1) {
    ll_on = 1;
    ll_cur_idex[0] = 4;
    ll_cur_idex[1] = random(0,4);
    ll_cur = LED_location[ll_cur_idex[0]][ll_cur_idex[1]];
    //Serial.print("Started new LED: "); Serial.println(ll_cur);
  }

  // Change colors
  for (int iset=0; iset < NUM_LEDS; iset++) {
    if (iset == ll_cur) {
      // Add color to pixel
      leds[iset] += CRGB(LL_COLOR_CHANGE_R,LL_COLOR_CHANGE_G,LL_COLOR_CHANGE_B);
      //Serial.print("Current RGB("); Serial.print(ll_cur); Serial.print("): "); Serial.print(leds[iset].r); Serial.print(","); Serial.print(leds[iset].g); Serial.print(","); Serial.println(leds[iset].b);

    } else if (iset == ll_old && ll_old != -1) {
      // remove color from old pixels
      if (leds[ll_cur].r - LL_COLOR_ON_R * .80 >= 0.0 && leds[ll_cur].g - LL_COLOR_ON_G * .80 >= 0.0 && leds[ll_cur].b - LL_COLOR_ON_B * .80 >= 0.0) {
        leds[iset] -= CRGB(LL_COLOR_CHANGE_R * 1.5,LL_COLOR_CHANGE_G * 1.5,LL_COLOR_CHANGE_B * 1.5);
      }
      //Serial.print("Old     RGB("); Serial.print(ll_old); Serial.print("): "); Serial.print(leds[iset].r); Serial.print(","); Serial.print(leds[iset].g); Serial.print(","); Serial.println(leds[iset].b);

    } else {
      // set all other pixels
      leds[iset] = CRGB(0,0,15);
    }
  }
  FastLED.show();

  // Move the LED (if it hit top or bottom)
  if (ll_old == -1) {
    if (leds[ll_cur].r >= LL_COLOR_ON_R && leds[ll_cur].g >= LL_COLOR_ON_G && leds[ll_cur].b >= LL_COLOR_ON_B) {
      //Serial.print("moving led (1)");
      LL_move();
    }
  } else {
    if (leds[ll_old].r <= 0 && leds[ll_old].g <= 0 && leds[ll_old].b <= 0) {
      //Serial.print("moving led (2)");
      LL_move();
    }
  }

  //Serial.println();
}

void Animations::LL_move() {
  leds[ll_old] = CRGB(0,0,20);
  if (ll_cur_idex[0] == 0) {
      // if led is on the top row, move down
      ani_dir = 1;
    } else if (ll_cur_idex[0] == 4) {
      // if led is on the bottom row, move up
      ani_dir = -1;
    }
    ll_old_idex[0] = ll_cur_idex[0];
    ll_old_idex[1] = ll_cur_idex[1];
    ll_old = ll_cur;
    ll_cur_idex[0] += ani_dir;
    ll_cur_idex[1] += random(-1,2);
    if (ll_cur_idex[1] > 3) {
      ll_cur_idex[1] = 0;
    } else if (ll_cur_idex[1] < 0) {
      ll_cur_idex[1] = 3;
    }
    ll_cur = LED_location[ll_cur_idex[0]][ll_cur_idex[1]];
    leds[ll_cur] = CRGB::Black;
    
    //Serial.print("Moved LED");
}

void Animations::LL_reset() {
  for (int i=0; i < NUM_LEDS; i++) {
    leds[i] = CRGB::Black;
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