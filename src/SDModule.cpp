#include "SDModule.h"
// Variables
File myFile;

// Functions
void initializeSD(){
    Serial.println("Initializing SD module...");
    if (!SD.begin(SS)) {
        Serial.println("SD module initialization failed!");
        while (1);
    }
    Serial.println("SD module has initialized successfully.");
}

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
