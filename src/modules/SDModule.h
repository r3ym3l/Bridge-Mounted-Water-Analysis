#pragma once

#include <SD.h>

// Functions
bool sdInit();
bool writeToSD(String fileName, String text);
void printFiles();
void createFile(String fileName);
bool isFileEmpty(String fileName);