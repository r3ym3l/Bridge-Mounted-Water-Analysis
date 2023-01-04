#include "SpectralModule.h"

DFRobot_AS7341 as7341;

void setup(void)
{
	Serial.begin(115200);
	//Detect if IIC can communicate properly 
	while (as7341.begin() != 0) {
		Serial.println("IIC init failed, please check if the wire connection is correct");
		delay(1000);
	}
	//Integration time = (ATIME + 1) x (ASTEP + 1) x 2.78µs
	//Set the value of register ATIME(1-255), through which the value of Integration time can be calculated. The value represents the time that must be spent during data reading.
	as7341.setAtime(29);
	//Set the value of register ASTEP(0-65534), through which the value of Integration time can be calculated. The value represents the time that must be spent during data reading.
	as7341.setAstep(599);
	//Set gain value(0~10 corresponds to X0.5,X1,X2,X4,X8,X16,X32,X64,X128,X256,X512)
	as7341.setAGAIN(7);
	//Enable LED
	//as7341.enableLed(true);
	//Set pin current to control brightness (1~20 corresponds to current 4mA,6mA,8mA,10mA,12mA,......,42mA)
	//as7341.controlLed(10);
}

void loop(void)
{
	spectralChannels ch;
	int n = 10;
	
	for (int i = 0; i < n; i++)
	{
		long int start = millis();
		readSensor(as7341, ch);
		printTime(start);		// replace with RTC to get current time instead of time it takes to read data

		// write data to .csv file
	}

	processReadings(ch, n);

	Serial.println("");
	Serial.print("Largest Value: ");
	Serial.print(ch.maxIntensity);
	Serial.print(" at channel F");
	Serial.println(ch.largestCh);
	Serial.println("--------------------------------------");

	Serial.println("Waiting...");
	Serial.println("");
	delay(20000);	// delay for 20 seconds after 10 readings then repeat
}