#pragma once

#include <Arduino.h>

#include <sensors.h>

class DistanceSensor {
    public:
        int readDistance();
        void printDistance();
};