#pragma once

#include <Arduino.h>
#include <Wire.h>

#include <vector>
#include <string>
#include <sstream>
#include <iostream>

// cellular
#include <Notecard.h>
#include <stdlib.h>

// This is the unique Product Identifier for your device
#ifndef PRODUCT_UID
#define PRODUCT_UID "com.gmail.ereymel1:bmwam"		// "com.my-company.my-name:my-project"
#pragma message "PRODUCT_UID is not defined in this example. Please ensure your Notecard has a product identifier set before running this example or define it in code here. More details at https://dev.blues.io/tools-and-sdks/samples/product-uid"
#endif

#define myProductID PRODUCT_UID

// sensors
#include "sensors/SpectralSensor.h"
#include "sensors/DistanceSensor.h"
#include "sensors/TempSensor.h"

// modules
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
String fileHeader = "Battery Information (Pending),Distance(mm),F1(405-425nm),F2(435-455nm),F3(470-490nm),F4(505-525nm),F5(545-565nm),F6(580-600nm),F7(620-640nm),F8(670-690nm),NIR(900nm),Temperature(Celsius),Timestamp";
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