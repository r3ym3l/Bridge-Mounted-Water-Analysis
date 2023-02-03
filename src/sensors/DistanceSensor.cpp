#include "DistanceSensor.h"

/**
 * @brief Reads the distance measured by the ultrasonic sensor
 * Also disables sampling on the sensor to save battery
 *
 * @return int The measured distance
 */
int readDistance() 
{
    digitalWrite(ULTRASONIC_EN, HIGH);
    sleep_ms(RANGING_DURATION);
    digitalWrite(ULTRASONIC_EN, LOW);
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
    Serial.print(distance);
    Serial.println(" mm");
}