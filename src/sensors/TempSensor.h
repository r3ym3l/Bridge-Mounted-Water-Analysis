#pragma once

#include <Adafruit_SHT31.h>
#include <pinout.h>

void tempInit();
float readTemp();
void printTemp();