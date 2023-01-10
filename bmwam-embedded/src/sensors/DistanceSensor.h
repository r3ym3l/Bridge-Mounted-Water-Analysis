#pragma once

#include <Arduino.h>

#include <pinout.h>

class DistanceSensor {
public:
    int readDistance();
    void printDistance();
};