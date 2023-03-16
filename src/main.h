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

// Modbus Master
#include <ModbusMaster.h>

// This is the unique Product Identifier for your device
#ifndef PRODUCT_UID
#define PRODUCT_UID "com.gmail.ereymel1:bmwam"		// "com.my-company.my-name:my-project"
#pragma message "PRODUCT_UID is not defined in this example. Please ensure your Notecard has a product identifier set before running this example or define it in code here. More details at https://dev.blues.io/tools-and-sdks/samples/product-uid"
#endif

#define myProductID PRODUCT_UID

// charge controller
#define NUM_DATA_REGISTERS 30
#define MODBUS_ADDRESS 0xFF
#define STARTING_REGISTER 0x100
#define CAPACITY_IDX 0x00
#define BATTERY_VOLTAGE_IDX 0x01
#define CHARGING_CURRENT_IDX 0x02
#define LOAD_VOLTAGE_IDX 0x04
#define LOAD_CURRENT_IDX 0x05
#define SOLAR_VOLTAGE_IDX 0x07
#define SOLAR_CURRENT_IDX 0x08
#define CHARGE_TODAY_IDX 0x11
#define DISCHARGE_TODAY_IDX 0x12

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
unsigned long distanceReadInterval = 30000UL;
unsigned long currentMillis = 0;
bool doneHandle;

ModbusMaster node;
uint16_t data_registers[NUM_DATA_REGISTERS];

struct batteryInfo 
{
	uint16_t capacity = 0;
	float batteryVoltage = 0.0;
	float chargeCurrent = 0.0;
	float loadVoltage = 0.0;
	float loadCurrent = 0.0;
	float solarVoltage = 0.0;
	float solarCurrent = 0.0;
	float chargeToday = 0.0;
	float dischargeToday = 0.0;
};

bool SDConnected;
bool RTCConnected;

String fileNameFormat = "datalog.csv";
String fileNameDate = "";
const char *fileHeader = 
"Battery Capacity(%),"
"Battery Voltage(V),"
"Charge Current(A),"
"Load Voltage(V),"
"Load Current(A),"
"Solar Panel Voltage(V),"
"Solar Panel Current(A),"
"Charge Today(Ah),"
"Discharge Today(Ah),"
"Distance(mm),"
"F1(405-425nm),"
"F2(435-455nm),"
"F3(470-490nm),"
"F4(505-525nm),"
"F5(545-565nm),"
"F6(580-600nm),"
"F7(620-640nm),"
"F8(670-690nm),"
"NIR(900nm),"
"Temperature(Celsius),"
"Timestamp";

char input;
int state;

const char *menuString = R"""(|   Commands    |
|h: Print commands menu
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

enum {
    setDistanceInterval = 0,
    setSpectralInterval,
    setTimeDate,
    toggleDistance,
    toggleSpectral
} commands;
