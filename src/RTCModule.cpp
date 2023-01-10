#include "RTCModule.h"

// Variables
RTC_DS3231 rtc;

// Functions
bool initializeRTC(){
  // serial.println("Initializing RTC module...");
    if (! rtc.begin()) {
        // serial.println("RTC module initialization failed.");
        // serial.flush();
        return false;
    }
    // serial.println("RTC module has initialized successfully.");
    return true;
}

String getTimeString()
{
  DateTime now = rtc.now();
  String timeString = "";
  timeString += now.year();
  timeString += '/';
  timeString += now.month();
  timeString += '/';
  timeString += now.day();
  timeString += ' ';
  timeString += now.hour();
  timeString += ':';
  timeString += now.minute();
  timeString += ':';
  timeString += now.second();
  return timeString;
}

void setTime(uint16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second)
{
  rtc.adjust(DateTime(year, month, day, hour, minute, second));
}


