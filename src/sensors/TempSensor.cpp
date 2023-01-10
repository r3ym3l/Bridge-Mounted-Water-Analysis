#include "TempSensor.h"

Adafruit_SHT31 sht30;

/**
 * @brief Initializes the temp sensor
 * 
 */
void tempInit() {
    if (!sht30.begin(SHT30_I2C_ADDRESS)) {
        Serial.println("Couldn't find SHT30");
        while (1) delay(1);
    }
}


/**
 * @brief Prints a measurement from the temp sensor
 *
 */
void printTemp() {
    float temp = sht30.readTemperature();
    if (!isnan(temp)) {
        Serial.print("temp = ");
        Serial.print(temp);
        Serial.println(" *C");
    }
}

