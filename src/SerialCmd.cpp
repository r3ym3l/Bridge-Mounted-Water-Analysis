#include "SerialCmd.h"
#include <sstream>
#include <iostream>
#include <Arduino.h>

const char *menuString = R"""(|   Commands    |
|h: Print commands menu
|0: Toggle Sensor On or Off
|1: Read Current Values
|2: Print Stored File
|3: Set Interval
|4: Adjust Time
)""";

void printMenu(){
    std::string line;
    std::istringstream iss(menuString);
    while (std::getline (iss, line))
    {
        Serial.println(line.c_str());
    }
    
}