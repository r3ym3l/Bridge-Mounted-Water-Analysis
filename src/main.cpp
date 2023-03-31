#include "main.h"

// cellular
Notecard notecard;

// A sample binary object, just for binary payload simulation
struct myBinaryPayload {
	batteryInfo bi;
	int distance;
	spectralChannels ch;
    int temperature;
    char * timestamp;
};

void cellularSetup();
void cellularLog(batteryInfo bi, int distance, spectralChannels ch, float temp, char * timestamp);
batteryInfo batteryInfoTask(char* sdLog);

// sensor tasks
int distanceTask(char* sdLog);
float tempTask(char* sdLog);
spectralChannels spectralTask(char* sdLog);

// miscellaneous functions
void cmdHandler();
void printMultiString(const char* toPrint);

void setup(void)
{
	doneHandle = 1;
	currentMillis = millis();
	previousMillis = currentMillis;

    Serial.begin(115200);

	// if Serial is not initialized and 10 seconds passed
	// keep program running 
    while (!Serial.available() && ((currentMillis - previousMillis) < 10*1000))
	{
		currentMillis = millis();
	}

	// sensor setups
	spectralInit();
	tempInit();
	SDConnected = sdInit();
	RTCConnected = rtcInit();
	//cellularSetup();

	if (Serial.available())
	{
		currentMillis = millis();
		previousMillis = currentMillis;
		state = 0;
		printMultiString(menuString);
		while (!doneHandle || ((currentMillis - previousMillis) < 20*1000))
		{
			currentMillis = millis();
			cmdHandler();
		}
	}

	// Initialize Charge Controller Modbus
	Serial.println("Initializing Charge Controller Modbus");
	_UART1_.begin(9600);
	node.begin(MODBUS_ADDRESS, _UART1_);
	delay(1000);
	Serial.println("Charge Controller Modbus initialized successfully");
	
	// only uses date as the file name
	int index = getTimeString().indexOf(' ');
	fileNameDate = getTimeString().substring(0, index);

	createFile(fileNameDate);

	Serial.print("fileNameDate: ");
	Serial.println(fileNameDate);
	Serial.print("Is fileNameFormat file empty? ");
	Serial.println(isFileEmpty(fileNameFormat));
	Serial.print("Is fileNameDate file empty? ");
	Serial.println(isFileEmpty(fileNameDate));
	
	// checks if file has no data, then add headers
	if (isFileEmpty(fileNameFormat))
	{
		Serial.println("Adding headers to csv file 1");
		writeToSD(fileNameFormat, fileHeader);
	}

	if (isFileEmpty(fileNameDate))
	{
		Serial.println("Adding headers to csv file 2");
		writeToSD(fileNameDate, fileHeader);
	}

	if(RTCConnected)
	{
		Serial.print("Date and Time is: ");
		Serial.println(getTimeString());
	}
}

void loop(void)
{
	//currentMillis = millis();
	//if (currentMillis - previousMillis > distanceReadInterval)
	//{
		if (!SDConnected)
		{
			if (sdInit())
			{
				SDConnected = true;
				Serial.println("SD card is connected.");
			}
		}

		char sdLog[150];

		// Timestamp
		String rtcString = getTimeString();
		rtcString.toCharArray(sdLog, rtcString.length()+1);
		//rtcString.toCharArray(rtc, rtcString.length()+1);
		


		batteryInfo bi = batteryInfoTask(sdLog);

		// sensor tasks
		int distance = distanceTask(sdLog);
		spectralChannels ch = spectralTask(sdLog);
		float temp = tempTask(sdLog);


		// Write to log to SD card
		if(!writeToSD(fileNameFormat, sdLog) && !writeToSD(fileNameDate, sdLog))
		{
			SDConnected = false;
		}
		else
		{
			Serial.println("String logged into SD card:");
			Serial.println(sdLog);
		}

		//char rtc[20];


		//cellularLog(bi, distance, ch, temp, rtc);
		//previousMillis = currentMillis;
		sleep_ms(distanceReadInterval);
	//}
}

void cellularSetup()
{
	notecard.setDebugOutputStream(Serial);
	Wire.begin();
    notecard.begin();

	// "NoteNewRequest()" uses the bundled "J" json package to allocate a "req", which is a JSON object
    // for the request to which we will then add Request arguments.  The function allocates a "req"
    // request structure using malloc() and initializes its "req" field with the type of request.
    J *req = notecard.newRequest("hub.set");

    // This command (required) causes the data to be delivered to the Project on notehub.io that has claimed
    // this Product ID.  (see above)
    if (myProductID[0]) {
        JAddStringToObject(req, "product", myProductID);
    }
    // This command determines how often the Notecard connects to the service.
    // JAddStringToObject(req, "mode", "continuous");
	JAddStringToObject(req, "mode", "periodic");
    JAddNumberToObject(req, "outbound", 30);		// send out data every 60 seconds
	JAddNumberToObject(req, "inbound", 60);

    // Issue the request, telling the Notecard how and how often to access the service.
    // This results in a JSON message to Notecard formatted like:
    //     { "req"     : "service.set",
    //     "product" : myProductID,
    //     "mode"    : "continuous"
    //     }
    // Note that NoteRequest() always uses free() to release the request data structure, and it
    // returns "true" if success and "false" if there is any failure.
    notecard.sendRequest(req);

	// Create a template note that we will register.  This template note will look "similar" to
    // the notes that will later be added with note.add, in that the data types are used to
    // intuit what the ultimate field data types will be, and their maximum length.
    req = notecard.newRequest("note.add");
    if (req != NULL) {

        // Create the body for a template that will be used to send notes below
        J *body = JCreateObject();
        if (body != NULL) {

            // Define the JSON template
            JAddNumberToObject(body, "Battery Capacity(%)", 1);					// integer
			JAddNumberToObject(body, "Charge Current(A)", 1.1);					// float
			JAddNumberToObject(body, "Load Voltage(V)", 1.1);					// float
			JAddNumberToObject(body, "Load Current(A)", 1.1);					// float
			JAddNumberToObject(body, "Solar Panel Voltage(V)", 1.1);			// float
			JAddNumberToObject(body, "Solar Panel Current(A)", 1.1);			// float
			JAddNumberToObject(body, "Charge Today(Ah)", 1.1);					// float
			JAddNumberToObject(body, "Discharge Today(Ah)", 1.1);				// float
            JAddNumberToObject(body, "Distance(mm)", 1);          				// integer
			JAddNumberToObject(body, "F1(405-425nm)", 1);       				// integer
			JAddNumberToObject(body, "F2(435-455nm)", 1);       				// integer
			JAddNumberToObject(body, "F3(470-490nm)", 1);       				// integer
			JAddNumberToObject(body, "F4(505-525nm)", 1);       				// integer
			JAddNumberToObject(body, "F5(545-565nm)", 1);       				// integer
			JAddNumberToObject(body, "F6(580-600nm)", 1);       				// integer
			JAddNumberToObject(body, "F7(620-640nm)", 1);       				// integer
			JAddNumberToObject(body, "F8(670-690nm)", 1);       				// integer
			JAddNumberToObject(body, "NIR(900nm)", 1);       					// integer
            JAddNumberToObject(body, "Temperature(Celsius)", 1.1);       		// float
            JAddStringToObject(body, "Timestamp", "AAAAAAAAAAAAAAAA");   		// maximum string length

            // Add the body to the request
            JAddItemToObject(req, "body", body);
        }

        // Create a template of the payload that will be used to send notes below
        JAddNumberToObject(req, "length", sizeof(myBinaryPayload));

        // Register the template in the output queue notefile
        JAddStringToObject(req, "file", "sensors.qo");
        JAddBoolToObject(req, "template", true);
        notecard.sendRequest(req);
    }
}

void cellularLog(batteryInfo bi, int distance, spectralChannels ch, float temp, char * timestamp)
{
	// Add a binary data structure to the simulation
    struct myBinaryPayload binaryData;
	binaryData.bi = bi;
	binaryData.distance = distance;
	binaryData.ch = ch;
	binaryData.temperature = temp;
	binaryData.timestamp = timestamp;

    // Enqueue the measurement to the Notecard for transmission to the Notehub
    J *req = notecard.newRequest("note.add");
    if (req != NULL) {
        JAddStringToObject(req, "file", "sensors.qo");
		JAddBoolToObject(req, "sync", true);
        J *body = JCreateObject();
        if (body) {
            JAddNumberToObject(body, "Battery Capacity(%)", bi.capacity);			// integer
			JAddNumberToObject(body, "Charge Current(A)", bi.chargeCurrent);		// float
			JAddNumberToObject(body, "Load Voltage(V)", bi.loadVoltage);			// float
			JAddNumberToObject(body, "Load Current(A)", bi.loadCurrent);			// float
			JAddNumberToObject(body, "Solar Panel Voltage(V)", bi.solarVoltage);	// float
			JAddNumberToObject(body, "Solar Panel Current(A)", bi.solarCurrent);			// float
			JAddNumberToObject(body, "Charge Today(Ah)", bi.chargeToday);			// float
			JAddNumberToObject(body, "Discharge Today(Ah)", bi.dischargeToday);		// float
            JAddNumberToObject(body, "Distance(mm)", distance);          			// integer
			JAddNumberToObject(body, "F1(405-425nm)", ch.f1);       				// integer
			JAddNumberToObject(body, "F2(435-455nm)", ch.f2);       				// integer
			JAddNumberToObject(body, "F3(470-490nm)", ch.f3);       				// integer
			JAddNumberToObject(body, "F4(505-525nm)", ch.f4);      					// integer
			JAddNumberToObject(body, "F5(545-565nm)", ch.f5);      					// integer
			JAddNumberToObject(body, "F6(580-600nm)", ch.f6);      					// integer
			JAddNumberToObject(body, "F7(620-640nm)", ch.f7);      					// integer
			JAddNumberToObject(body, "F8(670-690nm)", ch.f8);      					// integer
			JAddNumberToObject(body, "NIR(900nm)", ch.nir);     					// integer
            JAddNumberToObject(body, "Temperature(Celsius)", temp);       			// float
            JAddStringToObject(body, "Timestamp", timestamp);   					// maximum string length

            // Add the body to the request
            JAddItemToObject(req, "body", body);
        }
		JAddBinaryToObject(req, "payload", &binaryData, sizeof(binaryData));
        notecard.sendRequest(req);
    }

	Serial.println("Waiting to connect to Notehub...");
    // Delay until the notecard connects to notehub
    //delay(30*1000);    // 30 seconds
}

int distanceTask(char* sdLog)
{
	char tempString[50];
	int dist = readDistance();
	
	Serial.print("Ultrasonic Distance Sensor:");
	printDistance();
	Serial.println("");

	snprintf_P(tempString, sizeof(tempString), PSTR(",%d"), dist);
	strncat(sdLog, tempString, strlen(tempString));
	return dist;
}

float tempTask(char* sdLog)
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
	return temp;
}

spectralChannels spectralTask(char* sdLog)
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
	Serial.print("Spectral Sensor:");
	printSpectrum(ch);

	// place in spectralTask function
	snprintf_P(tempString, sizeof(tempString), PSTR(",%d,%d,%d,%d,%d,%d,%d,%d,%d"), ch.f1, ch.f2, ch.f3, ch.f4, ch.f5, ch.f6, ch.f7, ch.f8, ch.nir);
	strncat(sdLog, tempString, strlen(tempString));


    Serial.println("--------------------------------------");

	Serial.println("Waiting...");
	Serial.println("");
    delay(3000);	// delay for 8 seconds after 10 readings then repeat
	return ch;
}

batteryInfo batteryInfoTask(char* sdLog)	// void for now, later on will return battery info data type
{
	// Initialize variables
	static uint32_t i;
	uint8_t j, result;
	batteryInfo bi;

	char tempString[100];

	i++;
	node.setTransmitBuffer(0, lowWord(i));
	node.setTransmitBuffer(1, highWord(i));
	result = node.readHoldingRegisters(STARTING_REGISTER, NUM_DATA_REGISTERS);
	if (result == node.ku8MBSuccess)
	{
		Serial.println("Successfully read the data registers!");
		for (j = 0; j < NUM_DATA_REGISTERS; j++)
		{
			data_registers[j] = node.getResponseBuffer(j);
		}
	}
	else
	{
		Serial.println("Failed to read the data registers...");
	}

	bi.capacity = data_registers[CAPACITY_IDX];
	bi.batteryVoltage = data_registers[BATTERY_VOLTAGE_IDX] * 0.1;
	bi.chargeCurrent = data_registers[CHARGING_CURRENT_IDX] * 0.01;
	bi.loadVoltage = data_registers[LOAD_VOLTAGE_IDX] * 0.1;
	bi.loadCurrent = data_registers[LOAD_CURRENT_IDX] * 0.01;
	bi.solarVoltage = data_registers[SOLAR_VOLTAGE_IDX] * 0.1;
	bi.solarCurrent = data_registers[SOLAR_CURRENT_IDX] * 0.01;
	bi.chargeToday = data_registers[CHARGE_TODAY_IDX] * 0.1;
	bi.dischargeToday = data_registers[DISCHARGE_TODAY_IDX] * 0.1;

	snprintf_P(tempString, sizeof(tempString), PSTR(",%d,%f,%f,%f,%f,%f,%f,%f,%f"), bi.capacity, bi.batteryVoltage, bi.chargeCurrent, bi.loadVoltage, bi.loadCurrent, bi.solarVoltage, bi.solarCurrent, bi.chargeToday, bi.dischargeToday);
	//Serial.println(tempString);
	strncat(sdLog, tempString, strlen(tempString));

	return bi;
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
						doneHandle = 0;
						state = 3;
						break;
					}
					case '4':
					{
						Serial.println('4');
						Serial.println("Set the Date and Time using the format: YYYY MM DD HH mm ss");
						Serial.setTimeout(20*1000);
						String input = Serial.readStringUntil(' ');
						int year = input.substring(0, 4).toInt();
						int month = input.substring(4, 6).toInt();
						int day = input.substring(6, 8).toInt();
						int hour = input.substring(8, 10).toInt();
						int minute = input.substring(10, 12).toInt();
						int second = input.substring(12, 14).toInt();
						Serial.println(input);
						setTime(year, month, day, hour, minute, second);
						Serial.print("Date and Time set to: ");
						Serial.println(getTimeString());
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
			doneHandle = 1;
		}
	}
}

void printMultiString(const char *toPrint)
{
	std::string line;
	std::istringstream iss(toPrint);
	while (std::getline(iss, line))
	{
		// Serial.println(line.c_str());
		Serial.println(line.c_str());
	}
}
