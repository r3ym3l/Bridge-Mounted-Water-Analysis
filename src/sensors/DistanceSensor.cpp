#include "DistanceSensor.h"

/**
 * @brief Reads the distance measured by the ultrasonic sensor
 *
 * @return int The measured distance
 */
int readDistance() 
{
    return pulseIn(ULTRASONIC_PIN, HIGH);
}

 
/**
 * @brief Prints a measurement from the ultrasonic sensor
 * 
 */
void printDistance() 
{
    int distance = readDistance();
    Serial.print("dist = ");
    Serial.print(String(distance));
    Serial.print(" mm");
}