// Wire Master Writer
// by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire library
// Writes data to an I2C/TWI slave device
// Refer to the "Wire Slave Receiver" example for use with this

// Created 29 March 2006

// This example code is in the public domain.


#include <Wire.h>
#include "rolade.h"

RoladeProperties rollade[4];

uint8_t _bytes[20];
size_t data_size;

void setup()
{
  Wire.begin(); // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output

  rollade[0].SetAutomaticInterval(1,2,3,4);
  rollade[0].Serialize(_bytes, &data_size);
  Serial.print("starting");
  
  for (char i = 0; i < data_size; i++) {
    Serial.println(_bytes[i], BIN); // Drei Bytes schreiben...
  }
}

void loop()
{
  // send cmd(0) and 3 bytes
  Wire.beginTransmission(0x50 >> 1); // transmit to device #4
  Wire.send(0);  // Buffer Startadresse setzen 	
  rollade[0].Serialize(_bytes, &data_size);
  
  for (char i = 0; i < data_size; i++) {
    Wire.send(_bytes[i]); // Drei Bytes schreiben...
 //   Serial.println(_bytes[i], BIN); // Drei Bytes schreiben...
  }
  Wire.endTransmission();    // stop transmitting
  // receive answer from slave 0x50
  Wire.beginTransmission(0x50 >> 1); // transmit to device #4
  Wire.send(0x00);             // sets register pointer to echo #1 register (0x02)
  Wire.endTransmission();      
  Wire.requestFrom(0x50 >> 1, data_size);    // request 6 bytes from slave device #2

  char cnt = 0;
  while(Wire.available())    // slave may send less than requested
  {
    char c = Wire.receive(); // receive a byte as character
   _bytes[cnt++] = c;   
    
  }
  rollade[0].Deserialize(_bytes);
    

  
}
