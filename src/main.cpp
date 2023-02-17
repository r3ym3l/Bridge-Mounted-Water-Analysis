#include <Arduino.h>
#include <ModbusMaster.h>

#define RXPIN 9
#define TXPIN 8

int num_data_registers = 30;
int num_info_registers = 17;
ModbusMaster node;


void setup() {
  Serial.begin(115200);
  while(!Serial)
    ; // Wait for Serial to initialize
  Serial.println("Serial bus has initialized");
  _UART1_.begin(9600);
  Serial.println("UART1 has been initialized");
  int modbus_address = 255;
  node.begin(modbus_address, _UART1_);
  delay(1000);

}

void loop() {
  Serial.println("Test");
  //Serial.println(Serial2.read());
  // if (Serial2.read() > 0)
  // {
  //   Serial.println("Input");
  // }
  static uint32_t i;
  uint8_t j, result;
  uint16_t data_registers[num_data_registers];
  uint16_t info_registers[num_info_registers];
  
  i++;
  
  // set word 0 of TX buffer to least-significant word of counter (bits 15..0)
  node.setTransmitBuffer(0, lowWord(i));  
  // set word 1 of TX buffer to most-significant word of counter (bits 31..16)
  node.setTransmitBuffer(1, highWord(i));
    
  //////////////////////////////
  // Read the 30 data registers
  //////////////////////////////
  result = node.readHoldingRegisters(0x100, num_data_registers);
  //result = node.readHoldingRegisters(0x100, 1);
  if (result == node.ku8MBSuccess)
  {
    Serial.println("Successfully read the data registers!");
    for (j = 0; j < num_data_registers; j++)
    {
      data_registers[j] = node.getResponseBuffer(j);
      Serial.println(data_registers[j]);
    }
    Serial.println("---");
  }
  else {
    Serial.print("Failed to read the data registers... ");
    Serial.println(result, HEX); // E2 is timeout
  }


  //////////////////////////////
  // Read the 17 info registers
  //////////////////////////////
  result = node.readHoldingRegisters(0x00A, num_info_registers);
  if (result == node.ku8MBSuccess)
  {
    Serial.println("Successfully read the info registers!");
    for (j = 0; j < num_info_registers; j++)
    {
      info_registers[j] = node.getResponseBuffer(j);
      Serial.println(info_registers[j]);
    }
    Serial.println("---");
    Serial.println();
  }
  else {
    Serial.print("Failed to read the info registers... ");
    Serial.println(result, HEX); // E2 is timeout
  }

   delay(1000);
  // put your main code here, to run repeatedly:
}