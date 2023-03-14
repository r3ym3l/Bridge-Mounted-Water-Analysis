#include "TempSensor.h"

Adafruit_SHT31 sht30;

/**
 * @brief Initializes the temp sensor
 * 
 */
void tempInit() 
{
    while (!sht30.begin(SHT30_I2C_ADDRESS)) 
    {
        printString("Couldn't find SHT30");
        delay(1000);
    }
}

/**
 * @brief reads a measurement from the temp sensor
 *
 */
float readTemp() 
{
    return sht30.readTemperature();
}

/**
 * @brief Prints a measurement from the temp sensor
 *
 */
void printTemp() 
{
    float temp = sht30.readTemperature();
    
    if (!isnan(temp)) 
    {
        printString("temp = ");
        printString(String(temp));
        printString(" *C");
    }
}