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

  // Create NTP Object
  WiFiUDP ntpUDP;
  NTP ntp(ntpUDP);

// Loop Vars
  #include "Common.h"
  #include "Animations.hpp"
  Animations ani;
  #define LOOP_TIME                  1000
  unsigned long next_loop      =        0;
  
// Schedule Vars
  // these schedules assume only the start
  int sched_wake[7][3]         =  { // {hour, min, sec}
                                  { 8, 0, 0},
                                  { 7, 0, 0},
                                  { 7, 0, 0},
                                  { 7, 0, 0},
                                  { 7, 0, 0},
                                  { 7, 0, 0},
                                  { 7, 0, 0}};
  int sched_sleep[7][3]        =  { // {hour, min, sec}
                                  {20, 0, 0},
                                  {20, 0, 0},
                                  {20, 0, 0},
                                  {20, 0, 0},
                                  {20, 0, 0},
                                  {20, 0, 0},
                                  {20, 0, 0}};
  int sched_fun[7][3]          =  { // {hour, min, sec}
                                  { 8, 0, 0},
                                  { 8, 0, 0},
                                  { 8, 0, 0},
                                  { 8, 0, 0},
                                  { 8, 0, 0},
                                  { 8, 0, 0},
                                  { 8, 0, 0}};
  int sched_state              =        0; // states: 0 - sleep, 1 - wake, 3 - fun

// FastLED Variables
  #define LED_PIN                     5  //D1
  CRGB leds[NUM_LEDS];

void handle_state() {
  switch (sched_state) {
    case 0: //sleep
      ani.ChangeLED(4);
      break;
    case 1: // wake
      ani.ChangeLED(1);
      break;
    case 2: // fun
      ani.ChangeLED(3);
    break;
  }
}

void handle_sched() {
  int weekday = ntp.weekDay();
  // Set Sleep State (0)
  if (sched_state != 0) {
    if (ntp.hours() >= sched_sleep[weekday][1] && ntp.minutes() >= sched_sleep[weekday][2] && ntp.seconds() >= sched_sleep[weekday][3]) {
      Serial.println("Setting sleep mode");
      sched_state = 0;
    }
  }
  // Set Wake State (1)
  if (sched_state != 1) {
    if (ntp.hours() >= sched_wake[weekday][1] && ntp.minutes() >= sched_wake[weekday][2] && ntp.seconds() >= sched_wake[weekday][3]) {
      Serial.println("setting wake mode");
      sched_state = 1;
    }
  }
  // Set Fun State (2)
  if (sched_state != 2) {
    if (ntp.hours() >= sched_fun[weekday][1] && ntp.minutes() >= sched_fun[weekday][2] && ntp.seconds() >= sched_fun[weekday][3]) {
      Serial.println("setting fun mode");
      sched_state = 2;
    }
  }


  handle_state();
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("Starting Sleep Light");

  // FastLED
  Serial.println("Setting up LEDS");
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(10);
  ani.ChangeLED(1);

  // WiFiManager
  WiFiManager wifiManager;
  wifiManager.autoConnect("SleepLight");
  Serial.println("connected... yay!");
  ani.ChangeLED(2);

  // NTP Handling
  Serial.println("Setting up NTP");
  ntp.updateInterval(900000); // set to update from ntp server every 900 seconds, or 15 minutes
  ntp.ntpServer("north-america.pool.ntp.org");
  ntp.ruleDST("CDT", Second, Sun, Mar, 2, -300);
  ntp.ruleSTD("CST",  First, Sun, Nov, 3, -360);
  ntp.begin();


  ani.ChangeLED(3);
  FastLED.setBrightness(50);
  delay(1000);

  next_loop = millis() + LOOP_TIME;
}

void loop() {
  if (millis() >= next_loop) {
    ntp.update();
    handle_sched();
    Serial.println(ntp.formattedTime("%I:%M:%S"));
    next_loop = millis() + LOOP_TIME;
  }
  delay(1);
}