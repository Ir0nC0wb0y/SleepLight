#include <Arduino.h>

// Library Dependencies
#include <ESP8266WiFi.h>                           // Needed for WiFi
#include <ESP8266WebServer.h>                      // For WiFiManager
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
  //int time_hour_raw      =            0;
  //int time_hour          =            0;
  //int time_minute        =            0;
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
  #define LOOP_TIME                   250
  unsigned long next_loop      =        0;
  unsigned long ani_loop       =        0;
  #define WIFI_CHECK_TIME          900000    //Check WiFi state every 15 min
  unsigned long wifi_loop      =        0;
  
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
  int sched_state              =       -1; // states: 0 - sleep, 1 - wake, 3 - fun

// FastLED Variables
  #define LED_PIN                     5  //D1
  CRGB leds[NUM_LEDS];

void handle_state() {
  switch (sched_state) {
    case 0: //sleep
      Serial.println("Setting sleep mode");
      ani.ChangeLED(4);
      break;
    case 1: // wake
      Serial.println("setting wake mode");
      ani.ChangeLED(1);
      break;
    case 2: // fun
      Serial.println("setting fun mode");
      ani.RandomColors();
    break;
  }
}

void handle_sched() {
  int weekday = ntp.weekDay();
  int time_hr = ntp.hours();
  int time_mn = ntp.minutes();
  int time_sc = ntp.seconds();
  int state_prior = sched_state;
  if (time_sc != time_second) {
    Serial.print("Day: "); Serial.print(weekday); Serial.print(", hour: "); Serial.print(time_hr); Serial.print(", minute: "); Serial.print(time_mn); Serial.print(", seconds: "); Serial.println(time_sc);
    time_second = time_sc;
  }

  // Sorted by my own schedule, otherwise dealing with a race condition
  if (time_hr >= sched_wake[weekday][0] && time_mn >= sched_wake[weekday][1] && time_sc >= sched_wake[weekday][2]) {
    sched_state = 1;
  }
  if (time_hr >= sched_fun[weekday][0] && time_mn >= sched_fun[weekday][1] && time_sc >= sched_fun[weekday][2]) {
    sched_state = 2;
  }
  if (time_hr >= sched_sleep[weekday][0] && time_mn >= sched_sleep[weekday][1] && time_sc >= sched_sleep[weekday][2]) {
    sched_state = 0;
  }

  if (state_prior != sched_state) {
    switch (sched_state) {
      case 0:
        Serial.print("Sleep hour:    "); Serial.print(sched_sleep[weekday][0]); Serial.print(", minute: "); Serial.print(sched_sleep[weekday][1]); Serial.print(", seconds: "); Serial.println(sched_sleep[weekday][2]);
        break;
      case 1:
        Serial.print("Wake hour:     "); Serial.print(sched_wake[weekday][0]); Serial.print(", minute: "); Serial.print(sched_wake[weekday][1]); Serial.print(", seconds: "); Serial.println(sched_wake[weekday][2]);
        break;
      case 2:
        Serial.print("Wake hour:     "); Serial.print(sched_fun[weekday][0]); Serial.print(", minute: "); Serial.print(sched_fun[weekday][1]); Serial.print(", seconds: "); Serial.println(sched_fun[weekday][2]);
        break;
    }
    handle_state();
  }
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
  FastLED.setBrightness(20);
  delay(1000);

  next_loop = millis() + LOOP_TIME;
  ani_loop = millis();
  wifi_loop = millis() + WIFI_CHECK_TIME;
}

void loop() {
  if (millis() >= next_loop) {
    ntp.update();
    handle_sched();
    //Serial.println(ntp.formattedTime("%I:%M:%S"));
    next_loop = millis() + LOOP_TIME;
  }
  if (millis() >= ani_loop) {
    ani.HandleDisplay();
    ani_loop = millis() + ani.ani_refresh;
  }
  if (millis() >= wifi_loop) {
    if (WiFi.status() != WL_CONNECTED) {
      ani.Siren();
    }
  }
  delay(1);
}