#include <Arduino.h>

void setup() {
  Serial.begin(9600);
  pinMode(8, INPUT);
  pinMode(13, OUTPUT);
}

void loop() {
  if(digitalRead(8)){
    digitalWrite(13, HIGH);
  }else{
    digitalWrite(13, LOW);
  }
  delay(100);
}
