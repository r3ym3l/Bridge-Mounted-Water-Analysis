#pragma once

#include "DFRobot_AS7341.h"
struct spectralChannels {
	int f1 = 0;
	int f2 = 0;
	int f3 = 0;
	int f4 = 0;
	int f5 = 0;
	int f6 = 0;
	int f7 = 0;
	int f8 = 0;
	int clear = 0;
	int nir = 0;
	int maxIntensity = 0;
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