#include "SDModule.h"
// Variables
File myFile;
SdVolume volume;
SdFile root;
Sd2Card card;

uint SD_CS_PIN = 12u;

// Functions
bool initializeSD(){
    // serial.println("Initializing SD module...");
    if (!SD.begin(SD_CS_PIN)) {
        // serial.println("SD module initialization failed!");
        return false;
    }
    // serial.println("SD module has initialized successfully.");
    while (!card.init(SPI_HALF_SPEED, SD_CS_PIN)) {
    // serial.println("initialization failed. Things to check:");
    // serial.println("* is a card is inserted?");
    // serial.println("* Is your wiring correct?");
    // serial.println("* did you change the chipSelect pin to match your shield or module?");
    }  
    if (!volume.init(card)) {
      // serial.println("Could not find FAT16/FAT32 partition.");
      // serial.println("Make sure you've formatted the card");
      return false;
    }
    return true;
}

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
      // serial.println("Write to SD failed");
      return false;
    }
}

void printFiles()
{
  // // serial.println("Files (name, date and size in bytes): ");
  root.openRoot(volume);
  root.ls(LS_R | LS_DATE | LS_SIZE);
}

uint32_t readFileSize(String fileName)
{
  myFile = SD.open(fileName, FILE_READ);
  return myFile.size();
}
