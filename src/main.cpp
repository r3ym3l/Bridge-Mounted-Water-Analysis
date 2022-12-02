#include <Arduino.h>
#include "pinHeader.h"
#include <SPI.h>
#include <SD.h>
#include <vector>
#include <string>
#include "Wire.h"
#include "RTClib.h"

long duration;
int distance;

unsigned long previousMillis = 0UL;
unsigned long distanceReadInterval = 2000UL;
unsigned long currentMillis = 0;

File myFile;
File root;

String fileNameFormat = "datalog.csv";

RTC_DS3231 rtc;
void writeToSD(String fileName, String text)
{
    myFile = SD.open(fileName, FILE_WRITE);
    if (myFile)
    {
      myFile.println(text);
      myFile.close();
    }
    else
    {
      Serial.println("Write to SD failed");
    }
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

void setup() {
  // Initialize LED pin.
  pinMode(LED_BUILTIN, OUTPUT);

  // Initialize Distance Sensor pins.
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  // Initialize Serial comms.
  Serial.begin(96000);
  while (!Serial) {
    ; // Allow Serial to initialize
  }
  Serial.println("Serial has initialized...");

  // Initiallize SD Card. 
  Serial.println("Initializing SD card...");

  if (!SD.begin(SS)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("SD card initialization done.");
  Serial.println("Adding headers to csv file");
  
  writeToSD(fileNameFormat, "Time (ms),Distance (cm)");

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }
  Serial.println("RTC initialization done.");
  Serial.print("Time is:");
  Serial.println(getTimeString());
  Serial.println("test");
}

// the loop routine runs over and over again forever:
void loop() {
  currentMillis = millis();
  
  if(currentMillis - previousMillis > distanceReadInterval)
  {
    //DateTime now = rtc.now();
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
    Serial.println(distance);
    //myFile = SD.open("datalog.csv");
    char s[32];
    snprintf_P(s, sizeof(s), PSTR("%d,%d"),currentMillis,distance);
    Serial.println("Writing to file: " + fileNameFormat);
    writeToSD(fileNameFormat, s);
    previousMillis = currentMillis;
  }
}

