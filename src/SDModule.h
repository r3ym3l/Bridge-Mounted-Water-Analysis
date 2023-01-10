#include <SD.h>
// Variables
// Functions
bool initializeSD();
bool writeToSD(String fileName, String text);
void printFiles();
uint32_t readFileSize(String fileName);