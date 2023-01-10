#include "main.h"

// sensor tasks
void spectralTask();
void distanceTask();

// miscellaneous functions
void printTime(int time);

void setup(void)
{
	Serial.begin(115200);
	while (!Serial)
        delay(10);

	// sensor setups
	spectralInit();
	tempInit();
}

void loop(void)
{
  	// sensor tasks
	spectralTask();
	distanceTask();
}

void spectralTask()
{
  	spectralChannels ch;
	int n = 10;
	
	for (int i = 0; i < n; i++)
	{
		long int start = millis();
		printSpectrum(ch);
		printTime(start);		// replace with RTC to get current time instead of time it takes to read data

		// write data to .csv file
	}

  	// can save values in struct ch into .csv and/or
  	// save highest value (channel)
	processSpectrum(ch, n);

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

void distanceTask()
{
	printDistance();
	printTemp();

    Serial.println("-----");
    delay(1000);
}

void printTime(int time)
{
	Serial.print("Time to read data: ");
	Serial.print(time);
	Serial.println(" milliseconds");
	Serial.println("");
	delay(1000);
}
