#include <Arduino.h>
#include "pinHeader.h"
#include <SPI.h>
#include <vector>
#include <string>
#include "Wire.h"
#include "RTCModule.h"
#include "SDModule.h"
#include "SerialCmd.h"

long duration;
int distance;

unsigned long previousMillis = 0UL;
unsigned long distanceReadInterval = 2000UL;
unsigned long currentMillis = 0;

File root;

String fileNameFormat = "datalog.csv";
char input;

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
  delay(500);
  Serial.println("Serial has initialized...");

  // initializeSD();
  // Serial.println("Adding headers to csv file");
  // writeToSD(fileNameFormat, "Time (ms),Distance (cm)");

  // initializeRTC();
  // Serial.print("Time is:");
  // Serial.println(getTimeString());
  printMenu();
}

// the loop routine runs over and over again forever:
void loop() {
  if (Serial.available() > 0){
    char input = Serial.read();
    switch(input){
      case 'h':
      case 'H':
      {
        Serial.println('h');
        printMenu();
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
        Serial.println("Here are your values: ");
        break;
      }
      case '2':
      {
        Serial.println('2');
        Serial.println("Here are your files: ");
        break;
      }
      case '3':
      {
        Serial.println('3');
        Serial.println("Which sensor would you like to adjust?... ");
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
  }
  // currentMillis = millis();
  
  // if(currentMillis - previousMillis > distanceReadInterval)
  // {
  //   // Clears the trigPin condition
  //   digitalWrite(TRIG_PIN, LOW);
  //   delayMicroseconds(2);
  //   // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  //   digitalWrite(TRIG_PIN, HIGH);
  //   delayMicroseconds(10);
  //   digitalWrite(TRIG_PIN, LOW);
  //   // Reads the echoPin, returns the sound wave travel time in microseconds
  //   duration = pulseIn(ECHO_PIN, HIGH);
  //   // Calculating the distance
  //   distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  //   Serial.println(distance);
  //   char s[32];
  //   snprintf_P(s, sizeof(s), PSTR("%d,%d"),currentMillis,distance);
  //   Serial.println("Writing to file: " + fileNameFormat);
  //   writeToSD(fileNameFormat, s);
  //   previousMillis = currentMillis;
  // }
}