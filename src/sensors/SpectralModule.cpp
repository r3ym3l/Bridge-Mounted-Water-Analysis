#include "SpectralModule.h"

DFRobot_AS7341 as7341;

bool spectrumProcessed;

void spectralInit() 
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

	spectrumProcessed = false;
}

// getting the average intensity levels of each channel
// to determine the highest intensity (channel)
void processSpectrum(spectralChannels& ch, int n)
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
	ch.clear = ch.clear / (uint16_t) n;
	ch.nir = ch.nir / (uint16_t) n;
	
	ch.largestCh = ch.largestCh / n;
	ch.maxIntensity = ch.maxIntensity / (uint16_t) n;

	spectrumProcessed = true;
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
void readSpectrum(spectralChannels& ch)
{
	DFRobot_AS7341::sModeOneData_t data1;
	DFRobot_AS7341::sModeTwoData_t data2;

	//Start spectrum measurement 
	//Channel mapping mode: 1.eF1F4ClearNIR,2.eF5F8ClearNIR
	as7341.startMeasure(as7341.eF1F4ClearNIR);
	//Read the value of sensor data channel 0~5, under eF1F4ClearNIR
	data1 = as7341.readSpectralDataOne();
	
	ch.f1 = ch.f1 + data1.ADF1;
	ch.f2 = ch.f2 + data1.ADF2;
	ch.f3 = ch.f3 + data1.ADF3;
	ch.f4 = ch.f4 + data1.ADF4;   

	as7341.startMeasure(as7341.eF5F8ClearNIR);
	//Read the value of sensor data channel 0~5, under eF5F8ClearNIR
	data2 = as7341.readSpectralDataTwo();

	ch.f5 = ch.f5 + data2.ADF5;
	ch.f6 = ch.f6 + data2.ADF6;
	ch.f7 = ch.f7 + data2.ADF7;
	ch.f8 = ch.f8 + data2.ADF8;
	
	ch.clear = ch.clear + data2.ADCLEAR;
	ch.nir = ch.nir + data2.ADNIR;
}

// print spectral values
void printSpectrum(spectralChannels& ch)
{
	if (spectrumProcessed)
	{
		Serial.print("F1(405-425nm):");
		Serial.println(ch.f1);

		Serial.print("F2(435-455nm):");
		Serial.println(ch.f2);

		Serial.print("F3(470-490nm):");
		Serial.println(ch.f3);

		Serial.print("F4(505-525nm):");
		Serial.println(ch.f4);   

		Serial.print("F5(545-565nm):");
		Serial.println(ch.f5);

		Serial.print("F6(580-600nm):");
		Serial.println(ch.f6);

		Serial.print("F7(620-640nm):");
		Serial.println(ch.f7);

		Serial.print("F8(670-690nm):");
		Serial.println(ch.f8);
		
		Serial.print("Clear:");
		Serial.println(ch.clear);
		Serial.print("NIR:");
		Serial.println(ch.nir);
	}
}