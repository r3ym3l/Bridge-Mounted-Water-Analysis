#include "DFRobot_AS7341.h"

struct spectralChannels {
	float f1 = 0;
	float f2 = 0;
	float f3 = 0;
	float f4 = 0;
	float f5 = 0;
	float f6 = 0;
	float f7 = 0;
	float f8 = 0;
	float nir = 0;
};

void processReadings(spectralChannels& ch, int n);
void turbidityNorm(spectralChannels& ch);
void processAverage(spectralChannels& ch, int n);
void readSpectra(DFRobot_AS7341 as7341, spectralChannels& ch);
void printSpectra(spectralChannels& ch);