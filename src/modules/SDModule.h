#pragma once

#include <SD.h>

// Functions
bool sdInit();
bool writeToSD(String fileName, String text);
void printFiles();
bool isFileEmpty(String fileName);