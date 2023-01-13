#include "main.h"

// sensor tasks
void spectralTask();
void distanceTask();

// miscellaneous functions
void cmdHandler();
void printMultiString(const char* toPrint);
void printTime(int time);

void setup(void)
{
	Serial.begin(115200);
	while (!Serial)
    delay(10);

  // Initialize LED pin.
  pinMode(LED_BUILTIN, OUTPUT);
  // Initialize Distance Sensor pins.
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

	// sensor setups
	spectralInit();
	tempInit();
  SDConnected = sdInit();
  RTCConnected = rtcInit();

  if(!(readFileSize(fileNameFormat) > 0))
  {
    Serial.println("Adding headers to csv file");
    writeToSD(fileNameFormat, "Time (ms),Distance (cm)");
  }

  if(RTCConnected)
  {
    Serial.print("Date and Time is:");
    Serial.println(getTimeString());
  }
  state = 0;
  printMultiString(menuString);
}

void loop(void)
{
  // Handle any commands from the user
  // cmdHandler();

  if (!SDConnected){
    if (sdInit())
    {
      SDConnected = true;
    }
  }

  currentMillis = millis();
  char s[32];
	
  // *******************APPLY RTC TO DATA LOG****************
  // sensor tasks
	distanceTask();
  spectralTask();

  // if (currentMillis - previousMillis > distanceReadInterval)
  // {
  //   Serial.println(distanceReadInterval);
  //   // Clears the trigPin condition
  //   digitalWrite(TRIG_PIN, LOW);
  //   delayMicroseconds(2);
  //   // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  //   digitalWrite(TRIG_PIN, HIGH);
  //   delayMicroseconds(10);
  //   digitalWrite(TRIG_PIN, LOW);
  //   // Reads the echoPin, returns the sound wave travel time in microseconds
  //   duration = pulseIn(ECHO_PIN, HIGH);
  //   // Calculating the distance
  //   distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  //   Serial.println(distance);
    
  //   snprintf_P(s, sizeof(s), PSTR("%d,%d"), currentMillis, distance);
  //   if(!writeToSD(fileNameFormat, s))
  //   {
  //     SDConnected = false;
  //   }
  //   // Clearing the array
  //   s[0] = '\0';
  //   previousMillis = currentMillis;
  // }
}

void spectralTask()
{
  spectralChannels ch;
	int n = 10;
  char s[32];
	
	for (int i = 0; i < n; i++)
	{
		long int start = millis();
		readSpectrum(ch);
		printTime(start);		// replace with RTC to get current time instead of time it takes to read data
	}

  // can save values in struct ch into .csv and/or
  // save highest value (channel)
	processSpectrum(ch, n);

  // place in spectralTask function
  snprintf_P(s, sizeof(s), PSTR("%d,%d,%d,%d,%d,%d,%d,%d,%d"), currentMillis, ch.f1, ch.f2, ch.f3, ch.f4, ch.f5, ch.f6, ch.f7, ch.f8);
  if(!writeToSD(fileNameFormat, s))
  {
    SDConnected = false;
  }
  // Clearing the array
  s[0] = '\0';

	Serial.println("");
	Serial.print("Largest Value: ");
	Serial.print(ch.maxIntensity);
	Serial.print(" at channel F");
	Serial.println(ch.largestCh);
	Serial.println("--------------------------------------");

	Serial.println("Waiting...");
	Serial.println("");
	delay(8000);	// delay for 8 seconds after 10 readings then repeat
}

void distanceTask()
{
  char s[32];

	printDistance();
	printTemp();

  float temp = readTemp();
  int dist = readDistance();

  // check interval validity

  // if temp is a valid value, print it in SD
  if (!isnan(temp))
  {
    snprintf_P(s, sizeof(s), PSTR("%d,%d,%.3f"), currentMillis, dist, temp);
    if(!writeToSD(fileNameFormat, s))
    {
      SDConnected = false;
    }
  }
  else 
  {
    snprintf_P(s, sizeof(s), PSTR("%d,%d"), currentMillis, dist);
    if(!writeToSD(fileNameFormat, s))
    {
      SDConnected = false;
    }
  }

  // Clearing the array
  s[0] = '\0';

  Serial.println("-----");
  delay(1000);
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
            if(RTCConnected){
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
    // serial.println(line.c_str());
  }
}

void printTime(int time)
{
	Serial.print("Time to read data: ");
	Serial.print(time);
	Serial.println(" milliseconds");
	Serial.println("");
	delay(1000);
}
