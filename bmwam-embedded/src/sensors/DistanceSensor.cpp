#include "DistanceSensor.h"

/**
 * @brief Reads the distance measured by the ultrasonic sensor
 *
 * @return int The measured distance
 */
int DistanceSensor::readDistance() {
    return pulseIn(ULTRASONIC_PIN, HIGH);
}

 
/**
 * @brief Prints a measurement from the ultrasonic sensor
 * 
 */
void DistanceSensor::printDistance() {
    int distance = readDistance();
    Serial.print("dist = ");
    Serial.print(distance);
    Serial.println(" mm");
}

