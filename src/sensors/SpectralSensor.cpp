#include "SpectralSensor.h"

DFRobot_AS7341 as7341;

bool spectrumProcessed;

void spectralInit() 
{
    //Detect if IIC can communicate properly 
	while (as7341.begin() != 0) {
		printString("IIC init failed, please check if the wire connection is correct");
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
}

// normalizing spectra (700-900nm) to correct for skylight reflection
// void turbidityNorm(spectralChannels& ch)
// {
// 	ch.f8 = ch.f8 / (float) ch.f4;
// 	ch.nir = ch.nir / (float) ch.f4;
// }

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
	ch.nir = ch.nir / (uint16_t) n;

	spectrumProcessed = true;
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

	ch.nir = ch.nir + data2.ADNIR;
}

// print spectral values
void printSpectrum(spectralChannels& ch)
{
	if (spectrumProcessed)
	{
		printString("F1(405-425nm):" + String(ch.f1));
		printString("F2(435-455nm):" + String(ch.f2));
		printString("F3(470-490nm):" + String(ch.f3));
		printString("F4(505-525nm):" + String(ch.f4));
		printString("F5(545-565nm):" + String(ch.f5));
		printString("F6(580-600nm):" + String(ch.f6));
		printString("F7(620-640nm):" + String(ch.f7));
		printString("F8(670-690nm):" + String(ch.f8));
		printString("NIR:" + String(ch.nir));
	}
}