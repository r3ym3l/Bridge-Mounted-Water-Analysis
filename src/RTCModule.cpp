#include "RTCModule.h"

// Variables
RTC_DS3231 rtc;

// Functions
void initializeRTC(){
  Serial.println("Initializing RTC module...");
    if (! rtc.begin()) {
        Serial.println("RTC module initialization failed.");
        Serial.flush();
        while (1) delay(10);
    }
    Serial.println("RTC module has initialized successfully.");
}

String getTimeString()
{
  DateTime now = rtc.now();
  String timeString = "";
  timeString += now.year();
  timeString += '/';
  timeString += (now.month(), DEC);
  timeString += '/';
  timeString += (now.day(), DEC);
  timeString += ' ';
  timeString += (now.hour(), DEC);
  timeString += ':';
  timeString += (now.minute(), DEC);
  timeString += ':';
  timeString += (now.second(), DEC);
  return timeString;
}


