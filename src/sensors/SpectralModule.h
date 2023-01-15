#pragma once

#include "DFRobot_AS7341.h"
struct spectralChannels {
	uint16_t f1 = 0;
	uint16_t f2 = 0;
	uint16_t f3 = 0;
	uint16_t f4 = 0;
	uint16_t f5 = 0;
	uint16_t f6 = 0;
	uint16_t f7 = 0;
	uint16_t f8 = 0;
	uint16_t clear = 0;
	uint16_t nir = 0;
	uint16_t maxIntensity = 0;
	int largestCh;
	String strCh;
	int fCount[8];
};

void spectralInit();
void processSpectrum(spectralChannels& ch, int n);
void processAverage(spectralChannels& ch, int n);
void findMaxIntensity(spectralChannels& ch);
void readSpectrum(spectralChannels& ch);
void printSpectrum(spectralChannels& ch);