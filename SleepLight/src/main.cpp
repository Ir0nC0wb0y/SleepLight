#include <Arduino.h>

// Library Dependencies
#include <ESP8266WiFi.h>                           // Needed for WiFi
#include <WiFiUdp.h>                               // Needed for NTP

// WifiManager Setup
#include <WiFiManager.h>

// FastLED Setup
#define FASTLED_INTERRUPT_RETRY_COUNT 0
#define FASTLED_ESP8266_RAW_PIN_ORDER
#include <FastLED.h>

// NTPClient Setup
#include <NTP.h>
  // Time Keeping (NTP)
  long utcOffsetInSeconds_DST  = -18000;          // for local CDT
  long utcOffsetInSeconds      = -21600;          // for local CST
  #define NTP_UPDATE_INT         900000           // update interval for internal time to NTP servers
  //char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
  int time_hour_raw      =            0;
  int time_hour          =            0;
  int time_minute        =            0;
  int time_second        =            0;
  //int date_month         =            0;
  //int date_day           =            0;

  // DST Routines
  bool DST_flag          =            0;
  bool dst_state         =            1;               // default DST state
    // DST START
      // DST starts on the second sunday of March
      #define DST_START_MONTH           3
      #define DST_START_DAY             0
      #define DST_START_SEQ             2
      #define DST_START_HOUR            2
    // DST END
      // DST Ends the first sunday of November
      #define DST_END_MONTH            11
      #define DST_END_DAY               0
      #define DST_END_SEQ               1
      #define DST_END_HOUR              2

  // Create NTP Object
  WiFiUDP ntpUDP;
  NTP ntp(ntpUDP);

// Loop Vars
  #include "Common.h"
  #include "Animations.hpp"
  #define LOOP_TIME                   250
  unsigned long next_loop      =        0;
  int ani_type                 =        1;
  int ani_pos                  =        0;
  int ani_dir                  =        1;

// FastLED Variables
  #define LED_PIN                     5  //D1
  CRGB leds[NUM_LEDS];

void ChangeLED(int switcheroo) {
  switch (switcheroo) { 
    case 1:
      // Green
      for(int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed++) {
        // Turn our current led on to white, then show the leds
        leds[whiteLed] = CRGB::Green;
      }
      Serial.println("Changing LEDS to Green");
      break;
    case 2:
      // Blue
      for(int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed++) {
        // Turn our current led on to white, then show the leds
        leds[whiteLed] = CRGB::Blue;
      }
      Serial.println("Changing LEDS to Blue");
      break;
    case 3:
      // Purple
      for(int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed++) {
        // Turn our current led on to white, then show the leds
        leds[whiteLed] = CRGB::Purple;
      }
      Serial.println("Changing LEDS to Purple");
      break;
    case 4:
      // White
      for(int whiteLed = 0; whiteLed < NUM_LEDS; whiteLed++) {
        // Turn our current led on to white, then show the leds
        leds[whiteLed] = CRGB::White;
      }
      Serial.println("Changing LEDS to White");
      break;

  }
   FastLED.show();
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("Starting Sleep Light");

  // FastLED
  Serial.println("Setting up LEDS");
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(10);
  ChangeLED(1);

  // WiFiManager
  WiFiManager wifiManager;
  wifiManager.autoConnect("SleepLight");
  Serial.println("connected... yay!");
  ChangeLED(2);

  delay(500);
  // NTP Handling
  Serial.println("Setting up NTP");
  ntp.updateInterval(900000); // set to update from ntp server every 900 seconds, or 15 minutes
  ntp.ntpServer("north-america.pool.ntp.org");
  ntp.ruleDST("CDT", Second, Sun, Mar, 2, -300);
  ntp.ruleSTD("CST",  First, Sun, Nov, 3, -360);
  ntp.begin();
  ChangeLED(3);

  delay(500);
  ChangeLED(4);
  FastLED.setBrightness(50);
  delay(1000); 

  next_loop = millis() + LOOP_TIME;
}

void loop() {
  ntp.update();
  if (millis() >= next_loop) {
    //Serial.print("Ani_type: "); Serial.println(ani_type);
    switch (ani_type) {
      case 1:
        // Rotating
        Serial.print(ani_pos);
        RotatingColors(ani_pos);
        ani_pos = ani_pos + ani_dir;
        if (ani_pos >= LED_ROT) {
          ani_pos = 0;
          ani_dir = 1;
        } else if (ani_pos < 0) {
          ani_pos = 0;
          ani_dir = 1;
          //ani_type = 2;
        }
        Serial.print(" ");Serial.println(ani_pos);
        break;
      case 2:
        // Up-down
        ElevatingColors(ani_pos);
        ani_pos = ani_pos + ani_dir;
        if (ani_pos >= LED_HGT) {
          ani_pos = LED_HGT-2;
          ani_dir = -1;
        } else if (ani_pos < 0) {
          ani_pos = 0;
          ani_dir = 1;
          ani_type = 1;
        }
        break;
    }
    next_loop = millis() + LOOP_TIME;
  }
  delay(1);
}