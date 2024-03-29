#include "SDModule.h"
// Variables
File myFile;
SdVolume volume;
SdFile root;
Sd2Card card;

uint SD_CS_PIN = 12u;

// Functions
bool sdInit()
{
    Serial.println("Initializing SD module...");
    if (!SD.begin(SD_CS_PIN)) {
        Serial.println("SD module initialization failed!");
        return false;
    }
    Serial.println("SD module has initialized successfully.");
    while (!card.init(SPI_HALF_SPEED, SD_CS_PIN)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("* is a card is inserted?");
    Serial.println("* Is your wiring correct?");
    Serial.println("* did you change the chipSelect pin to match your shield or module?");
    }  
    if (!volume.init(card)) {
      Serial.println("Could not find FAT16/FAT32 partition.");
      Serial.println("Make sure you've formatted the card");
      return false;
    }
    return true;
}

/*If the same SPI bus is shared with other peripherals, it is important that the SD
card be initialized before accessing any other peripheral on the bus. Failure to
do so can prevent the SD card from being recognized until it is powered off or reinserted.*/
bool writeToSD(String fileName, String text)
{
    myFile = SD.open(fileName, FILE_WRITE);
    if (myFile)
    {
      myFile.println(text);
      myFile.close();
      return true;
    }
    else
    {
      Serial.println("Write to SD failed");
      return false;
    }
}

void printFiles()
{
  // Serial.println("Files (name, date and size in bytes): ");
  root.openRoot(volume);
  root.ls(LS_R | LS_DATE | LS_SIZE);
}

uint32_t readFileSize(String fileName)
{
  myFile = SD.open(fileName, FILE_READ);
  return myFile.size();
}