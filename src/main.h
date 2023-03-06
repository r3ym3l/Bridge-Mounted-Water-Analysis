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