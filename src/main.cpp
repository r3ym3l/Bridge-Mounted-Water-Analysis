#include "main.h"

// sensor tasks
void distanceTask(char* sdLog);
void tempTask(char* sdLog);
void spectralTask(char* sdLog);

// miscellaneous functions
void cmdHandler();
void printMultiString(const char* toPrint);
void printTime(int time);

void setup(void)
{
    Serial.begin(115200);
    while (!Serial)
    delay(10);

	// sensor setups
	spectralInit();
	tempInit();
	SDConnected = sdInit();
	RTCConnected = rtcInit();

	// NEEDS FIXING
	// if(!(readFileSize(fileNameFormat) > 0))
	// {
	// 	Serial.println("Adding headers to csv file");
	// 	writeToSD(fileNameFormat, fileHeader);
	// }

	Serial.println("Adding headers to csv file");
	writeToSD(fileNameFormat, fileHeader);

	if(RTCConnected)
	{
		Serial.print("Date and Time is: ");
		Serial.println(getTimeString());
	}
	state = 0;
	printMultiString(menuString);
}

void loop(void)
{
	// Handle any commands from the user
	Serial.println("");
	cmdHandler();

	if (!SDConnected)
	{
		if (sdInit())
		{
			SDConnected = true;
			Serial.println("SD card is connected.");
		}
	}

	currentMillis = millis();
	char sdLog[100];

	String rtcString = getTimeString();
	rtcString.toCharArray(sdLog, rtcString.length()+1);
	
	Serial.println("");
	// NEEDS UPDATE:
	// Do we need these read intervals? 
	// Current code still writes to SD with only RTC value due to the conditions not being met below
	if (currentMillis - previousMillis > distanceReadInterval)
	{
		// sensor tasks
		distanceTask(sdLog);
		tempTask(sdLog);
		spectralTask(sdLog);
		previousMillis = currentMillis;
	}

	Serial.println("String logged into SD card:");
	Serial.println(sdLog);

	// NEEDS UPDATE:
	// How often should we write to SD
	// Current code currently writes often to SD (everytime loop resets)
	if(!writeToSD(fileNameFormat, sdLog))
	{
		SDConnected = false;
	}

	// Clearing the array
  	sdLog[0] = '\0';
}

void distanceTask(char* sdLog)
{
	char tempString[50];
	int dist = readDistance();
	
	Serial.println("Ultrasonic Distance Sensor:");
	printDistance();
	Serial.println("");

	snprintf_P(tempString, sizeof(tempString), PSTR(",%d"), dist);
	strncat(sdLog, tempString, strlen(tempString));
}

void tempTask(char* sdLog)
{
	char tempString[50];
	Serial.println("Temperature Sensor:");
	printTemp();
	Serial.println("");

	float temp = readTemp();

	// if temp is a valid value, log it in SD
	if (!isnan(temp))
	{
		snprintf_P(tempString, sizeof(tempString), PSTR(",%.3f"), temp);
		strncat(sdLog, tempString, strlen(tempString));
	}
}

void spectralTask(char* sdLog)
{
	spectralChannels ch;
	int n = 2;
	char tempString[100];

    for (int i = 0; i < n; i++)
    {
        readSpectrum(ch);
    }

	// can save values in struct ch into .csv and/or
	// save highest value (channel)
	processSpectrum(ch, n);

	// will only print when the spectral values has been processed
	Serial.println("Spectral Sensor:");
	printSpectrum(ch);

	// place in spectralTask function
	snprintf_P(tempString, sizeof(tempString), PSTR(",%d,%d,%d,%d,%d,%d,%d,%d,%d"), ch.f1, ch.f2, ch.f3, ch.f4, ch.f5, ch.f6, ch.f7, ch.f8, ch.nir);
	strncat(sdLog, tempString, strlen(tempString));

    Serial.print("Largest Value: ");
    Serial.print(ch.maxIntensity);
    Serial.print(" at channel F");
    Serial.println(ch.largestCh);
    Serial.println("--------------------------------------");

    Serial.println("");
    // delay(8000);	// delay for 8 seconds after 10 readings then repeat
}

void cmdHandler()
{
	if (Serial.available() > 0)
	{
		char input = Serial.read();
		switch (state)
		{
			case 0:
			{
				switch (input)
				{
					case 'h':
					case 'H':
					{
						Serial.println('h');
						printMultiString(menuString);
						break;
					}
					case '0':
					{
						Serial.println('0');
						Serial.println("Which sensor would you like to toggle?... ");
						break;
					}
					case '1':
					{
						Serial.println('1');
						if(RTCConnected)
						{
							Serial.print("Date and Time is: ");
							Serial.println(getTimeString());
						}
						else
						{
							Serial.println("RTC Module not initialized.");
						}
						break;
					}
					case '2':
					{
						if(SDConnected)
						{
							Serial.println('2');
							printFiles();
						}
						else
						{
							Serial.println("SD Card not initialized.");
						}
						break;
					}
					case '3':
					{
						Serial.println('3');
						Serial.println("Set Sensor Read Interval Time: ");
						printMultiString(intervalString);
						state = 3;
						break;
					}
					case '4':
					{
						Serial.println('4');
						Serial.println("Set the Date and Time using this format... ");
						break;
					}
					default:
					{
						Serial.println("Unkown command, press h to view all commands. ");
						break;
					}
				}
				break;
			}
			// State 3, Set Interval Time
			case 3:
			{
				switch(input)
				{
					case '1':
					{
						Serial.println("Interval Set to: 5 Seconds");
						distanceReadInterval = 5000UL;
						state = 0;
						break;
					}
					case '2':
					{
						Serial.println("Interval Set to: 30 Seconds");
						distanceReadInterval = 30000UL;
						state = 0;
						break;
					}
					case '3':
					{
						Serial.println("Interval Set to: 2 Minutes");
						distanceReadInterval = 120000UL;
						state = 0;
						break;
					}
					case '4':
					{
						Serial.println("Interval Set to: 5 Seconds");
						distanceReadInterval = 3600000UL;
						state = 0;
						break;
					}
					default:
					{
						Serial.println("Invalid Input. Here Are Your Options: ");
						printMultiString(intervalString);
						break;
					}
				}
				break;
			}
		
			default:
			{
				break;
			}
		}
	}
}

void printMultiString(const char *toPrint)
{
	std::string line;
	std::istringstream iss(toPrint);
	while (std::getline(iss, line))
	{
		Serial.println(line.c_str());
	}
}

void printString(String toPrint)
{
	if (Serial.available() > 0){
		Serial.println(toPrint);
	}
}
