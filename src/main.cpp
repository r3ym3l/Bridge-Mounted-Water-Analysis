#include <Arduino.h>
#include "PinHeader.h"
#include <SPI.h>
#include <vector>
#include <string>
#include "Wire.h"
#include "RTCModule.h"
#include "SDModule.h"
#include <sstream>
#include <iostream>

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

const char *intervalMenu = R"""(
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

void cmdHandler();
void printMultiString(const char* toPrint);

void setup()
{
  // Initialize LED pin.
  pinMode(LED_BUILTIN, OUTPUT);
  // Initialize Distance Sensor pins.
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Initialize // serial comms.
  //// serial.begin(96000);
  // while (!// serial)
  // {
  //   ; // Allow // serial to initialize
  // }
  delay(500);
  // serial.println("// serial has initialized...");

  SDConnected = initializeSD();
  if(!(readFileSize(fileNameFormat) > 0))
  {
    // // serial.println("Adding headers to csv file");
    writeToSD(fileNameFormat, "Time (ms),Distance (cm)");
  }

  // while (!card.init(SPI_HALF_SPEED, 12u)) {
  //   // serial.println("initialization failed. Things to check:");
  //   // serial.println("* is a card is inserted?");
  //   // serial.println("* Is your wiring correct?");
  //   // serial.println("* did you change the chipSelect pin to match your shield or module?");
  // } 



  RTCConnected = initializeRTC();
  if(RTCConnected)
  {
    // // serial.print("Date and Time is:");
    // // serial.println(getTimeString());
  }
  state = 0;
  printMultiString(menuString);
}

// the loop routine runs over and over again forever:
void loop()
{
  // Handle any commands from the user
  //cmdHandler();
  
  if(!SDConnected){
    if(initializeSD())
    {
      SDConnected = true;
    }
  }

  currentMillis = millis();

  if (currentMillis - previousMillis > distanceReadInterval)
  {
    // // serial.println(distanceReadInterval);
    // Clears the trigPin condition
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    // Reads the echoPin, returns the sound wave travel time in microseconds
    duration = pulseIn(ECHO_PIN, HIGH);
    // Calculating the distance
    distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
    // // serial.println(distance);
    char s[32];
    snprintf_P(s, sizeof(s), PSTR("%d,%d"), currentMillis, distance);
    if(!writeToSD(fileNameFormat, s))
    {
      SDConnected = false;
    }
    previousMillis = currentMillis;
  }
}

void cmdHandler()
{
  if (Serial.available() > 0)
  {
    char input = Serial.read();
    switch (state)
    {
      case 0:
      {
        switch (input)
        {
          case 'h':
          case 'H':
          {
            Serial.println('h');
            printMultiString(menuString);
            break;
          }
          case '0':
          {
            Serial.println('0');
            Serial.println("Which sensor would you like to toggle?... ");
            break;
          }
          case '1':
          {
            Serial.println('1');
            if(RTCConnected){
              Serial.print("Date and Time is: ");
              Serial.println(getTimeString());
            }
            else
            {
              Serial.println("RTC Module not initialized.");
            }
            break;
          }
          case '2':
          {
            if(SDConnected)
            {
              Serial.println('2');
              printFiles();
            } 
            else
            {
              Serial.println("SD Card not initialized.");
            }
            break;
          }
          case '3':
          {
            Serial.println('3');
            Serial.println("Set Sensor Read Interval Time: ");
            printMultiString(intervalString);
            state = 3;
            break;
          }
          case '4':
          {
            Serial.println('4');
            Serial.println("Set the Date and Time using this format... ");
            break;
          }
          default:
          {
            Serial.println("Unkown command, press h to view all commands. ");
            break;
          }
        }
        break;
      }
      // State 3, Set Interval Time
      case 3:
      {
        switch(input)
        {
          case '1':
          {
            Serial.println("Interval Set to: 5 Seconds");
            distanceReadInterval = 5000UL;
            state = 0;
            break;
          }
          case '2':
          {
            Serial.println("Interval Set to: 30 Seconds");
            distanceReadInterval = 30000UL;
            state = 0;
            break;
          }
          case '3':
          {
            Serial.println("Interval Set to: 2 Minutes");
            distanceReadInterval = 120000UL;
            state = 0;
            break;
          }
          case '4':
          {
            Serial.println("Interval Set to: 5 Seconds");
            distanceReadInterval = 3600000UL;
            state = 0;
            break;
          }
          default:
          {
            Serial.println("Invalid Input. Here Are Your Options: ");
            printMultiString(intervalString);
            break;
          }
        }
        break;
      }
      default:
      {
        break;
      }
    }
  }
}

void printMultiString(const char *toPrint)
{
  std::string line;
  std::istringstream iss(toPrint);
  while (std::getline(iss, line))
  {
    // serial.println(line.c_str());
  }
}