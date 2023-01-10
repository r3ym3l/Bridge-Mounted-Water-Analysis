#include "TempSensor.h"

/**
 * @brief Initializes the temp sensor
 * 
 */
void TempSensor::init() {
    if (!begin(SHT30_I2C_ADDRESS)) {
        Serial.println("Couldn't find SHT30");
        while (1) delay(1);
    }
}


/**
 * @brief Prints a measurement from the temp sensor
 *
 */
void TempSensor::printTemp() {
    float temp = readTemperature();
    if (!isnan(temp)) {
        Serial.print("temp = ");
        Serial.print(temp);
        Serial.println(" *C");
    }
}

