#include "main.h"

DistanceSensor ultrasonic = DistanceSensor();

void setup() {
    Serial.begin(115200);
}

void loop() {
    ultrasonic.printDistance();
    delay(1000);
}