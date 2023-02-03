#pragma once

#include <Arduino.h>
#include <pinout.h>

#define RANGING_DURATION 200

int readDistance();
void printDistance();