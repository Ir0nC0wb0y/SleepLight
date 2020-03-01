#include <Arduino.h>

// Library Dependencies
#include <ESP8266WiFi.h>                           // Needed for WiFi
#include <WiFiUdp.h>                               // Needed for NTP

// NTPClient Setup
#include <NTP.h>
  // Time Keeping (NTP)
  long utcOffsetInSeconds_DST  = -18000;          // for local CDT
  long utcOffsetInSeconds      = -21600;          // for local CST
  #define NTP_UPDATE_INT         900000           // update NTP reckoning to servers
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

void setup() {
  Serial.begin(115200);
  Serial.println("Starting Sleep Light");

  // NTP Handling
  ntp.updateInterval(900000); // set to update from ntp server every 900 seconds, or 15 minutes
  ntp.ntpServer("north-america.pool.ntp.org");
  ntp.ruleDST("CDT", Second, Sun, Mar, 2, -300);
  ntp.ruleSTD("CST",  First, Sun, Nov, 3, -360);
  ntp.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  ntp.update();
  Serial.print("Epoch: "); Serial.println(ntp.epoch());
  delay(10000);
}