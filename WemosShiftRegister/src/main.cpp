#include <Arduino.h>
#include <Shifty.h>

// Declare the shift register
Shifty shift; 

//Generates the clock signal to control the transference of data
int clockPin = D8; //D8 (=GPIO15) --> SRCLK (=SH_CP)
//Controls the internal transference of data in SN74HC595 internal registers 
int loadPin = D7; //D7 (=GPIO13) --> RCLK (=ST_CP)
//Outputs the byte to transfer
int dataPin = D6;//D6 (=GPIO12) --> SER (=DS)
 
void setup() 
{
  Serial.begin(115200);
    // Set the number of bits you have (multiples of 8)
  shift.setBitCount(8);

  // Set the clock, data, and latch pins you are using
  // This also sets the pinMode for these pins
  shift.setPins(dataPin, clockPin, loadPin); 
}

void loop() {
  // writeBit works just like digitalWrite
  shift.writeBit(0, HIGH);
  delay(1000);
  shift.writeBit(1, HIGH);
  delay(1000);
  shift.writeBit(2, HIGH);
  delay(1000);
  shift.writeBit(3, HIGH);
  delay(1000);
  shift.writeBit(0, LOW);
  delay(1000);
  shift.writeBit(1, LOW);
  delay(1000);
  shift.writeBit(2, LOW);
  delay(1000);
  shift.writeBit(3, LOW);
 
  delay(1000);
}
