#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SHT31.h>

#include <pinout.h>

class TempSensor: public Adafruit_SHT31 {

public:
    void init();
    void printTemp();
};
