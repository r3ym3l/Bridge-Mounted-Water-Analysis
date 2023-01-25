#include "SpectralModule.h"

// getting the average intensity levels of each channel
// to determine the highest intensity (channel)
void processReadings(spectralChannels& ch, int n)
{
	processAverage(ch, n);
	findMaxIntensity(ch);
}

// average intensity levels of each channel for "n" readings
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

// read spectral values
void readSensor(DFRobot_AS7341 as7341, spectralChannels& ch)
{
	DFRobot_AS7341::sModeOneData_t data1;
	DFRobot_AS7341::sModeTwoData_t data2;

	//Start spectrum measurement 
	//Channel mapping mode: 1.eF1F4ClearNIR,2.eF5F8ClearNIR
	as7341.startMeasure(as7341.eF1F4ClearNIR);

	//Put the sensor into measurement mode by gpio pulse 
  	while(!as7341.measureComplete());
	
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

	//Put the sensor into measurement mode by gpio pulse 
  	while(!as7341.measureComplete());

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