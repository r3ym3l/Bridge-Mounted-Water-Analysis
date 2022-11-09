#include "DFRobot_AS7341.h"

DFRobot_AS7341 as7341;

struct spectralChannels {
	uint16_t f1 = 0;
	uint16_t f2 = 0;
	uint16_t f3 = 0;
	uint16_t f4 = 0;
	uint16_t f5 = 0;
	uint16_t f6 = 0;
	uint16_t f7 = 0;
	uint16_t f8 = 0;
	uint16_t maxIntensity = 0;
	int largestCh;
	String strCh;
	int fCount[8];
};

void printTime(int time);
void processReadings(spectralChannels& ch, int n);
void processAverage(spectralChannels& ch, int n);
void findMaxIntensity(spectralChannels& ch);
void readSensor(spectralChannels& ch);

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
		readSensor(ch);
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

void printTime(int time)
{
	Serial.print("Time to read data: ");
	Serial.print(time);
	Serial.println(" milliseconds");
	Serial.println("");
	delay(1000);
}

void processReadings(spectralChannels& ch, int n)
{
	processAverage(ch, n);
	findMaxIntensity(ch);
}

void processAverage(spectralChannels& ch, int n)
{
	ch.f1 = ch.f1 / (uint16_t) n;
	ch.f2 = ch.f2 / (uint16_t) n;
	ch.f3 = ch.f3 / (uint16_t) n;
	ch.f4 = ch.f4 / (uint16_t) n;
	ch.f5 = ch.f5 / (uint16_t) n;
	ch.f6 = ch.f6 / (uint16_t) n;
	ch.f7 = ch.f7 / (uint16_t) n;
	ch.f8 = ch.f8 / (uint16_t) n;
	
	ch.largestCh = ch.largestCh / n;
	ch.maxIntensity = ch.maxIntensity / (uint16_t) n;
}

// find largest intensity level
void findMaxIntensity(spectralChannels& ch)
{
	if (ch.f1 > ch.maxIntensity)
	{
		ch.maxIntensity = ch.f1;
		ch.largestCh = 1;
		ch.strCh = "Channel F1 has the largest intensity level";
	}
	if (ch.f2 > ch.maxIntensity)
	{
		ch.maxIntensity = ch.f2;
		ch.largestCh = 2;
		ch.strCh = "Channel F2 has the largest intensity level";
	}
	if (ch.f3 > ch.maxIntensity)
	{
		ch.maxIntensity = ch.f3;
		ch.largestCh = 3;
		ch.strCh = "Channel F3 has the largest intensity level";
	}
	if (ch.f4 > ch.maxIntensity)
	{
		ch.maxIntensity = ch.f4;
		ch.largestCh = 4;
		ch.strCh = "Channel F4 has the largest intensity level";
	}
	if (ch.f5 > ch.maxIntensity)
	{
		ch.maxIntensity = ch.f5;
		ch.largestCh = 5;
		ch.strCh = "Channel F5 has the largest intensity level";
	}
	if (ch.f6 > ch.maxIntensity)
	{
		ch.maxIntensity = ch.f6;
		ch.largestCh = 6;
		ch.strCh = "Channel F6 has the largest intensity level";
	}
	if (ch.f7 > ch.maxIntensity)
	{
		ch.maxIntensity = ch.f7;
		ch.largestCh = 7;
		ch.strCh = "Channel F7 has the largest intensity level";
	}
	if (ch.f8 > ch.maxIntensity)
	{
		ch.maxIntensity = ch.f8;
		ch.largestCh = 8;
		ch.strCh = "Channel F8 has the largest intensity level";
	}
}

// 
void readSensor(spectralChannels& ch)
{
	DFRobot_AS7341::sModeOneData_t data1;
	DFRobot_AS7341::sModeTwoData_t data2;

	//Start spectrum measurement 
	//Channel mapping mode: 1.eF1F4ClearNIR,2.eF5F8ClearNIR
	as7341.startMeasure(as7341.eF1F4ClearNIR);
	//Read the value of sensor data channel 0~5, under eF1F4ClearNIR
	data1 = as7341.readSpectralDataOne();
	
	Serial.print("F1(405-425nm):");
	Serial.println(data1.ADF1);
	ch.f1 = ch.f1 + data1.ADF1;

	Serial.print("F2(435-455nm):");
	Serial.println(data1.ADF2);
	ch.f2 = ch.f2 + data1.ADF2;

	Serial.print("F3(470-490nm):");
	Serial.println(data1.ADF3);
	ch.f3 = ch.f3 + data1.ADF3;

	Serial.print("F4(505-525nm):");
	Serial.println(data1.ADF4);
	ch.f4 = ch.f4 + data1.ADF4;   

	as7341.startMeasure(as7341.eF5F8ClearNIR);
	//Read the value of sensor data channel 0~5, under eF5F8ClearNIR
	data2 = as7341.readSpectralDataTwo();
	Serial.print("F5(545-565nm):");
	Serial.println(data2.ADF5);
	ch.f5 = ch.f5 + data2.ADF5;

	Serial.print("F6(580-600nm):");
	Serial.println(data2.ADF6);
	ch.f6 = ch.f6 + data2.ADF6;

	Serial.print("F7(620-640nm):");
	Serial.println(data2.ADF7);
	ch.f7 = ch.f7 + data2.ADF7;

	Serial.print("F8(670-690nm):");
	Serial.println(data2.ADF8);
	ch.f8 = ch.f8 + data2.ADF8;
	
	Serial.print("Clear:");
	Serial.println(data2.ADCLEAR);
	Serial.print("NIR:");
	Serial.println(data2.ADNIR);
}