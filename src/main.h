#pragma once

#include <Arduino.h>
#include <Wire.h>
#include "PinHeader.h"

#include <vector>
#include <string>
#include <sstream>
#include <iostream>

// sensors
#include "sensors/SpectralModule.h"
#include "sensors/DistanceSensor.h"
#include "sensors/TempSensor.h"
#include "sensors/RTCModule.h"
#include "sensors/SDModule.h"

long duration;
int distance;

unsigned long previousMillis = 0UL;
unsigned long distanceReadInterval = 2000UL;
unsigned long currentMillis = 0;

bool SDConnected;
bool RTCConnected;

String fileNameFormat = "datalog.csv";
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
