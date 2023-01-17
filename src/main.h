#pragma once

#include <Arduino.h>
#include <Wire.h>

#include <vector>
#include <string>
#include <sstream>
#include <iostream>

// sensors
#include "sensors/SpectralSensor.h"
#include "sensors/DistanceSensor.h"
#include "sensors/TempSensor.h"
#include "modules/RTCModule.h"
#include "modules/SDModule.h"

long duration;
int distance;

unsigned long previousMillis = 0UL;
unsigned long distanceReadInterval = 2000UL;
unsigned long currentMillis = 0;

bool SDConnected;
bool RTCConnected;

String fileNameFormat = "datalog.csv";
String fileHeader = "RTC Date (Year-Month-Day Hours:Minutes),Distance (mm),Temperature (Celsius),Blue,Light-Blue,Green-Blue,Green,Yellow,Orange,Pink,Red,Clear,NIR";
char input;
int state;

const char *menuString = R"""(|   Commands    |
|h: Print commands menu
|0: Toggle Sensor On or Off
|1: Print Date and Time
|2: Print Card Information
|3: Set Interval
|4: Adjust Time
)""";

const char *intervalString = R"""(
|1: 5 Seconds
|2: 30 Seconds
|3: 2 Minutes
|4: 1 Hour
)""";

const char *dateTimeMenu = R"""(
|1: Year
|2: Month
|3: Day
|4: Hour
|5: Minute
|6: Second
)""";

const char *headerString = 
"Timestamp, " 
"Distance (cm), " 
"Temperature (°C), " 
"Humidity, " 
"F1, F2, F3, F4, F5, F6, F7, F8, F9, "
"Battery Health";

enum {
    setDistanceInterval = 0,
    setSpectralInterval,
    setTimeDate,
    toggleDistance,
    toggleSpectral
} commands;