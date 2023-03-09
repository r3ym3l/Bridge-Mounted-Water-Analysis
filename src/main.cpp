#include "main.h"

DFRobot_AS7341 as7341;
Notecard notecard;

// A sample binary object, just for binary payload simulation
struct myBinaryPayload {
    char *timestamp;
	int distance;
	spectralChannels ch;
    int temperature;
	int batteryInfo;
};

void spectralSetup();
void cellularSetup();
spectralChannels spectralTask();
void datalog(spectralChannels ch);

void setup(void)
{
  	Serial.begin(115200);
  	// sensor setups
	spectralSetup();
	// module setups
	cellularSetup();
}

void loop(void)
{
  	// sensor tasks
	spectralChannels ch = spectralTask();

	datalog(ch);
}

void spectralSetup()
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
	as7341.startMeasure(as7341.eF1F4ClearNIR);
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
            JAddStringToObject(body, "Timestamp", "AAAAAAAAAAAAAAAA");   	// maximum string length
            JAddNumberToObject(body, "Distance(mm)", 1);          			// integer
            JAddNumberToObject(body, "Temperature(Celsius)", 1.1);       	// floating point (double)
			JAddNumberToObject(body, "F1(405-425nm)", 1.1);       			// floating point (double)
			JAddNumberToObject(body, "F2(435-455nm)", 1.1);       			// floating point (double)
			JAddNumberToObject(body, "F3(470-490nm)", 1.1);       			// floating point (double)
			JAddNumberToObject(body, "F4(505-525nm)", 1.1);       			// floating point (double)
			JAddNumberToObject(body, "F5(545-565nm)", 1.1);       			// floating point (double)
			JAddNumberToObject(body, "F6(580-600nm)", 1.1);       			// floating point (double)
			JAddNumberToObject(body, "F7(620-640nm)", 1.1);       			// floating point (double)
			JAddNumberToObject(body, "F8(670-690nm)", 1.1);       			// floating point (double)
			JAddNumberToObject(body, "NIR(900nm)", 1.1);       				// floating point (double)
            JAddNumberToObject(body, "Battery Information (Pending)", 1);	// integer

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

spectralChannels spectralTask()
{
  	spectralChannels ch;
	int n = 10;

	for (int i = 0; i < n; i++)
	{
		readSpectra(as7341, ch);
	}

  	// can save values in struct ch into .csv and/or
  	// save highest value (channel)
	processReadings(ch, n);
	printSpectra(ch);

	Serial.println("--------------------------------------");

	Serial.println("");
	delay(3000);	// delay for 3 seconds after n readings then repeat

	return ch;
}

void datalog(spectralChannels ch)
{
	// Add a binary data structure to the simulation
    struct myBinaryPayload binaryData;
	binaryData.timestamp = "2023-03-06 17:00";
	binaryData.distance = 1000;
	binaryData.temperature = 23.87;
	binaryData.ch = ch;
	binaryData.batteryInfo = 1;

    // Enqueue the measurement to the Notecard for transmission to the Notehub
    J *req = notecard.newRequest("note.add");
    if (req != NULL) {
        JAddStringToObject(req, "file", "sensors.qo");
		JAddBoolToObject(req, "sync", true);
        J *body = JCreateObject();
        if (body) {
            JAddStringToObject(body, "Timestamp", "2023-01-16 17:40");   	// maximum string length
            JAddNumberToObject(body, "Distance(mm)", 1000);          		// integer
            JAddNumberToObject(body, "Temperature(Celsius)", 23.87);       	// floating point (double)
			JAddNumberToObject(body, "F1(405-425nm)", ch.f1);       		// floating point (double)
			JAddNumberToObject(body, "F2(435-455nm)", ch.f2);       		// floating point (double)
			JAddNumberToObject(body, "F3(470-490nm)", ch.f3);       		// floating point (double)
			JAddNumberToObject(body, "F4(505-525nm)", ch.f4);      			// floating point (double)
			JAddNumberToObject(body, "F5(545-565nm)", ch.f5);      			// floating point (double)
			JAddNumberToObject(body, "F6(580-600nm)", ch.f6);      			// floating point (double)
			JAddNumberToObject(body, "F7(620-640nm)", ch.f7);      			// floating point (double)
			JAddNumberToObject(body, "F8(670-690nm)", ch.f8);      			// floating point (double)
			JAddNumberToObject(body, "NIR(900nm)", ch.nir);     			// floating point (double)
            JAddNumberToObject(body, "Battery Information (Pending)", 1);	// integer

            // Add the body to the request
            JAddItemToObject(req, "body", body);
        }
		JAddBinaryToObject(req, "payload", &binaryData, sizeof(binaryData));
        notecard.sendRequest(req);
    }

	Serial.println("Waiting to connect to Notehub...");
    // Delay until the notecard connects to notehub
    delay(30*1000);    // 30 seconds
}