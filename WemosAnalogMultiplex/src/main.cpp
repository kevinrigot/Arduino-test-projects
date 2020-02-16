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

int pin_Out_S0 = D0;
int pin_Out_S1 = D1;
int pin_Out_S2 = D2;
int pin_In_Mux1 = A0;
int Mux1_State[8] = {0};

void updateMux1 () {
  for (int i = 0; i < 8; i++){
    digitalWrite(pin_Out_S0, HIGH && (i & B00000001));
    digitalWrite(pin_Out_S1, HIGH && (i & B00000010));
    digitalWrite(pin_Out_S2, HIGH && (i & B00000100));
    Mux1_State[i] = analogRead(pin_In_Mux1);
  }
}
void logMux1(){
  for(int i = 0; i < 8; i ++) {
    if(i == 7) {
      Serial.println(Mux1_State[i]);
    } else {
      Serial.print(Mux1_State[i]);
      Serial.print(",");
    }
  }
}
int previousRate = 0;
int currentRateValue = 0;
void writeMux1(){
  int rate = map(Mux1_State[4], 0, 1024, 4000, 0);
  if(abs(previousRate - rate) > 100){
    currentRateValue = rate;
    previousRate = rate;
    Serial.print("New rate:");
    Serial.println(rate);
  }
  bool rateValue = LOW;
  if(currentRateValue < 10){
    rateValue = HIGH;
  }
  for(int i = 0; i <= 3; i ++) {
    if(Mux1_State[i] > 1000){
      shift.writeBit(i, rateValue);
    }else{
      shift.writeBit(i, LOW);
    }
  }
  currentRateValue--;
  if(currentRateValue < 0){
    currentRateValue = rate;
  }
}

void setup() {
  pinMode(pin_Out_S0, OUTPUT);
  pinMode(pin_Out_S1, OUTPUT);
  pinMode(pin_Out_S2, OUTPUT);
  pinMode(pin_In_Mux1, INPUT);
  Serial.begin(9600);

  shift.setBitCount(8);
  shift.setPins(dataPin, clockPin, loadPin); 
}

void loop() {
  updateMux1();
  // logMux1();
  writeMux1();
  
}
