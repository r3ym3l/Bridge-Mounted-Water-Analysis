#include "main.h"

DistanceSensor ultrasonic = DistanceSensor();
TempSensor sht30 = TempSensor();

void setup() {
    Serial.begin(9600);
    while (!Serial)
        delay(10);
    
    sht30.init();
    //   if (! sht30.begin(0x44)) {   // Set to 0x45 for alternate i2c addr
    //     Serial.println("Couldn't find SHT31");
    //     while (1) delay(1);
    //   }

    // pinMode(16, OUTPUT);
}

void loop() {
    ultrasonic.printDistance();
    sht30.printTemp();
    // digitalWrite(16, !digitalRead(16));

    Serial.println("-----");
    delay(1000);
}



