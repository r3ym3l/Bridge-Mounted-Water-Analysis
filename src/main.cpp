#include "sensors/SpectralModule.h"

DFRobot_AS7341 as7341;

void spectralSetup();
void spectralTask();
void printTime(int time);

void setup(void)
{
  Serial.begin(115200);
  // sensor setups
	spectralSetup();
}

void loop(void)
{
  // sensor tasks
	spectralTask();
}

void spectralSetup()
{
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
}

void spectralTask()
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

  // can save values in struct ch into .csv and/or
  // save highest value (channel)
	processReadings(ch, n);

	Serial.println("");
	Serial.print("Largest Value: ");
	Serial.print(ch.maxIntensity);
	Serial.print(" at channel F");
	Serial.println(ch.largestCh);
	Serial.println("--------------------------------------");

	Serial.println("Waiting...");
	Serial.println("");
	delay(8000);	// delay for 20 seconds after 10 readings then repeat
}

void printTime(int time)
{
	Serial.print("Time to read data: ");
	Serial.print(time);
	Serial.println(" milliseconds");
	Serial.println("");
	delay(1000);
}
