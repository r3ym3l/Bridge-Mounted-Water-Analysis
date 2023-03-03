#include "sensors/SpectralModule.h"

#include <Notecard.h>
#include <Wire.h>

#include <stdlib.h>

// This is the unique Product Identifier for your device
#ifndef PRODUCT_UID
#define PRODUCT_UID "com.gmail.ereymel1:bmwam"		// "com.my-company.my-name:my-project"
#pragma message "PRODUCT_UID is not defined in this example. Please ensure your Notecard has a product identifier set before running this example or define it in code here. More details at https://dev.blues.io/tools-and-sdks/samples/product-uid"
#endif

#define myProductID PRODUCT_UID

DFRobot_AS7341 as7341;
Notecard notecard;

void spectralSetup();
void cellularSetup();
void spectralTask();

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
	spectralTask();
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
    JAddStringToObject(req, "mode", "periodic");
    JAddNumberToObject(req, "outbound", 5);

    // Issue the request, telling the Notecard how and how often to access the service.
    // This results in a JSON message to Notecard formatted like:
    //     { "req"     : "service.set",
    //     "product" : myProductID,
    //     "mode"    : "continuous"
    //     }
    // Note that NoteRequest() always uses free() to release the request data structure, and it
    // returns "true" if success and "false" if there is any failure.
    notecard.sendRequest(req);
}

void spectralTask()
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

	Serial.println("Waiting...");
	Serial.println("");
	delay(3000);	// delay for 3 seconds after n readings then repeat

    // Enqueue the measurement to the Notecard for transmission to the Notehub
    J *req = notecard.newRequest("note.add");
    if (req != NULL) {
        JAddStringToObject(req, "file", "sensors.qo");
		JAddBoolToObject(req, "sync", true);
        J *body = JCreateObject();
        if (body != NULL) {
            JAddNumberToObject(body, "f1", ch.f1);
            JAddNumberToObject(body, "f2", ch.f2);
			JAddNumberToObject(body, "f3", ch.f3);
			JAddNumberToObject(body, "f4", ch.f4);
			JAddNumberToObject(body, "f5", ch.f5);
			JAddNumberToObject(body, "f6", ch.f6);
			JAddNumberToObject(body, "f7", ch.f7);
			JAddNumberToObject(body, "f8", ch.f8);
			JAddNumberToObject(body, "nir", ch.nir);
            JAddItemToObject(req, "body", body);
        }
        notecard.sendRequest(req);
    }

    // Delay between measurements
    delay(5*1000);    // 5 seconds
}