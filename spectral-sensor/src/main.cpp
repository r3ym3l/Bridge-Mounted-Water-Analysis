#include "DFRobot_AS7341.h"

DFRobot_AS7341 as7341;

void readSensor(void);
void printTime(int time);
void findMaxModeOne(DFRobot_AS7341::sModeOneData_t data, uint16_t* max);
void findMaxModeTwo(DFRobot_AS7341::sModeTwoData_t data, uint16_t* max);

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
	for (int i = 0; i < 10; i++)
	{
		long int start = millis();
		readSensor();
		printTime(start);		// replace with RTC to get current time instead of time it takes to read data

		// write data to .csv file
	}
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

void findMaxModeOne(DFRobot_AS7341::sModeOneData_t data, uint16_t* max)
{
	// find largest data
	if (data.ADF1 > *max)
		*max = data.ADF1;
	if (data.ADF2 > *max) 
		*max = data.ADF2;
	if (data.ADF3 > *max) 
		*max = data.ADF3;
	if (data.ADF4 > *max) 
		*max = data.ADF4;
}

void findMaxModeTwo(DFRobot_AS7341::sModeTwoData_t data, uint16_t* max)
{
	// find largest data
	if (data.ADF5 > *max)
		*max = data.ADF5;
	if (data.ADF6 > *max) 
		*max = data.ADF6;
	if (data.ADF7 > *max) 
		*max = data.ADF7;
	if (data.ADF8 > *max) 
		*max = data.ADF8;
}

void readSensor(void)
{
	DFRobot_AS7341::sModeOneData_t data1;
	DFRobot_AS7341::sModeTwoData_t data2;

	uint16_t max = 0;

	//Start spectrum measurement 
	//Channel mapping mode: 1.eF1F4ClearNIR,2.eF5F8ClearNIR
	as7341.startMeasure(as7341.eF1F4ClearNIR);
	//Read the value of sensor data channel 0~5, under eF1F4ClearNIR
	data1 = as7341.readSpectralDataOne();
	
	Serial.print("F1(405-425nm):");
	Serial.println(data1.ADF1);
	Serial.print("F2(435-455nm):");
	Serial.println(data1.ADF2);
	Serial.print("F3(470-490nm):");
	Serial.println(data1.ADF3);
	Serial.print("F4(505-525nm):");   
	Serial.println(data1.ADF4);

	as7341.startMeasure(as7341.eF5F8ClearNIR);
	//Read the value of sensor data channel 0~5, under eF5F8ClearNIR
	data2 = as7341.readSpectralDataTwo();
	Serial.print("F5(545-565nm):");
	Serial.println(data2.ADF5);
	Serial.print("F6(580-600nm):");
	Serial.println(data2.ADF6);
	Serial.print("F7(620-640nm):");
	Serial.println(data2.ADF7);
	Serial.print("F8(670-690nm):");
	Serial.println(data2.ADF8);
	Serial.print("Clear:");
	Serial.println(data2.ADCLEAR);
	Serial.print("NIR:");
	Serial.println(data2.ADNIR);

	// find largest value (colour/intensity level) and which channel it's from
	findMaxModeOne(data1, &max);
	findMaxModeTwo(data2, &max);

	Serial.println("");
	Serial.print("Largest Value: ");
	Serial.println(max);
	Serial.println("--------------------------------------");
}