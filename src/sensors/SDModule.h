#include <SD.h>

// Functions
bool sdInit();
bool writeToSD(String fileName, String text);
void printFiles();
uint32_t readFileSize(String fileName);