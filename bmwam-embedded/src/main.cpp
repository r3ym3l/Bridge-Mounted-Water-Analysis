#include "main.h"

DistanceSensor ultrasonic = DistanceSensor();
TempSensor sht30 = TempSensor();

void setup() {
    Serial.begin(9600);
    while (!Serial)
        delay(10);
    
    sht30.init();
}

void loop() {
    ultrasonic.printDistance();
    sht30.printTemp();

    Serial.println("-----");
    delay(1000);
}



