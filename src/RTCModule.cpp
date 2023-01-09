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


