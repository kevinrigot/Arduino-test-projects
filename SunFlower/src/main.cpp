#include <Arduino.h>
#include <Servo.h>

Servo servoVertical;
Servo servoHorizontal;

const String APP_NAME = "SunFlower - " __FILE__;
const String APP_VERSION = "v0.1-" __DATE__ " " __TIME__;

#define LOWER_LEFT A2
#define UPPER_LEFT A3
#define LOWER_RIGHT A1
#define UPPER_RIGHT A0
#define SERVO_VERTICAL 9
#define SERVO_HORIZONTAL 10

const int tolerance = 100;
const int step = 2;
const int minVertical = 60;
const int maxVertical = 5;
const int minHorizontal = 20;
const int maxHorizontal = 160;

int horizontalPosition = 90;
int verticalPosition = 20;
void setup() {
  Serial.begin(115200);
  Serial.println(APP_NAME);
  Serial.println(APP_VERSION);
  servoHorizontal.attach(SERVO_HORIZONTAL);
  servoVertical.attach(SERVO_VERTICAL);
  pinMode(LOWER_RIGHT, INPUT);
  pinMode(UPPER_RIGHT, INPUT);
  pinMode(LOWER_LEFT, INPUT);
  pinMode(UPPER_LEFT, INPUT);

  servoHorizontal.write(horizontalPosition);
  servoVertical.write(verticalPosition);
  delay(500);
}



void loop() {
  int valLowerRight = analogRead(LOWER_RIGHT);
  int valUpperRight = analogRead(UPPER_RIGHT);
  int valLowerLeft = analogRead(LOWER_LEFT);
  int valUpperLeft = analogRead(UPPER_LEFT);

  Serial.print("val valLowerRight: ");
  Serial.println(valLowerRight);
  Serial.print("val valUpperRight: ");
  Serial.println(valUpperRight);
  Serial.print("val valLowerLeft: ");
  Serial.println(valLowerLeft);
  Serial.print("val valUpperLeft: ");
  Serial.println(valUpperLeft);
  Serial.println("--------------------");

  int valLeft = valUpperLeft +valLowerLeft;
  int valRight = valUpperRight +valLowerRight;
  if(abs(valLeft - valRight) > tolerance){
    if(valLeft > valRight) {
      Serial.println("Rotate towards left");
      if(horizontalPosition > minHorizontal){
        horizontalPosition -= step;
      }
    }else{
      Serial.println("Rotate towards right");
      if(horizontalPosition < maxHorizontal){
        horizontalPosition += step;
      }
    }
    servoHorizontal.write(horizontalPosition);
  }else{
    Serial.println("Horizontal rotation OK");
  }
  int valUpper = valUpperLeft +valUpperRight;
  int valLower = valLowerLeft +valLowerRight;
  if(abs(valUpper - valLower) > tolerance){
    if(valUpper > valLower) {
      Serial.println("Flip Up");
      if(verticalPosition > maxVertical){
        verticalPosition -= step;
      }
    }else{
      Serial.println("Flip Down");
      if(verticalPosition < minVertical){
        verticalPosition += step;
      }
    }
    Serial.print("Vertical write: ");
    Serial.println(verticalPosition);
    servoVertical.write(verticalPosition);
  }else{
    Serial.println("Vertical rotation OK");
  }
  delay(50);
}