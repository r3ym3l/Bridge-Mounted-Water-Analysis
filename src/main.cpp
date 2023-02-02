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
	as7341.startMeasure(as7341.eF1F4ClearNIR);
}

void spectralTask()
{
  	spectralChannels ch;
	int n = 10;

	for (int i = 0; i < n; i++)
	{
		readSpectra(as7341, ch);
	}

  	// can save values in struct ch into .csv and/or
  	// save highest value (channel)
	processReadings(ch, n);
	printSpectra(ch);

	Serial.println("--------------------------------------");

	Serial.println("Waiting...");
	Serial.println("");
	delay(3000);	// delay for 3 seconds after n readings then repeat
}

void printTime(int time)
{
	Serial.print("Time to read data: ");
	Serial.print(time);
	Serial.println(" milliseconds");
	Serial.println("");
	delay(1000);
}
