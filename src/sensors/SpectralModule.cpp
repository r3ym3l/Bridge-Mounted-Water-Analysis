#include "SpectralModule.h"

// getting the average intensity levels of each channel
// to determine the highest intensity (channel)
void processReadings(spectralChannels& ch, int n)
{
	processAverage(ch, n);
	turbidityNorm(ch);
}

// normalizing spectra (700-900nm) to correct for skylight reflection
void turbidityNorm(spectralChannels& ch)
{
	ch.f8 = ch.f8 / (float) ch.f4;
	ch.nir = ch.nir / (float) ch.f4;
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
	ch.f8 = ch.f8 / (float) n;
	ch.nir = ch.nir / (float) n;
}

// read spectral values
void readSpectra(DFRobot_AS7341 as7341, spectralChannels& ch)
{
	DFRobot_AS7341::sModeOneData_t data1;
	DFRobot_AS7341::sModeTwoData_t data2;

	//Start spectrum measurement 
	//Channel mapping mode: 1.eF1F4ClearNIR,2.eF5F8ClearNIR
	as7341.startMeasure(as7341.eF1F4ClearNIR);

	//Put the sensor into measurement mode by gpio pulse 
  	// while(!as7341.measureComplete());
	
	//Read the value of sensor data channel 0~5, under eF1F4ClearNIR
	data1 = as7341.readSpectralDataOne();
	
	ch.f1 = ch.f1 + data1.ADF1;
	ch.f2 = ch.f2 + data1.ADF2;
	ch.f3 = ch.f3 + data1.ADF3;
	ch.f4 = ch.f4 + data1.ADF4;   

	as7341.startMeasure(as7341.eF5F8ClearNIR);
	//Read the value of sensor data channel 0~5, under eF5F8ClearNIR

	//Put the sensor into measurement mode by gpio pulse 
  	// while(!as7341.measureComplete());

	data2 = as7341.readSpectralDataTwo();

	ch.f5 = ch.f5 + data2.ADF5;
	ch.f6 = ch.f6 + data2.ADF6;
	ch.f7 = ch.f7 + data2.ADF7;
	ch.f8 = ch.f8 + data2.ADF8;
	ch.nir = ch.nir + data2.ADNIR;
}

void printSpectra(spectralChannels& ch)
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
	
	Serial.print("NIR:");
	Serial.println(ch.nir);
}