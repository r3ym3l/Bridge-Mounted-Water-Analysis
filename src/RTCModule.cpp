#include "RTCModule.h"

// Variables
RTC_DS3231 rtc;

// Functions
void initializeRTC(){
    if (! rtc.begin()) {
        Serial.println("Couldn't find RTC");
        Serial.flush();
        while (1) delay(10);
    }
    Serial.println("RTC initialization done.");
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


