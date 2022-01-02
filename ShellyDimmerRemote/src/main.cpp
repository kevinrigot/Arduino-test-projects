#pragma once
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <shelly-dimmer.h>

const String APP_NAME = "ShellyDimmerRemote - " __FILE__;
const String APP_VERSION = "v0.2-" __DATE__ " " __TIME__;

#define IS_ON_LED D1
#define TURN_ON_SWITCH D2
#define BRIGHTNESS_POT A0

int DEBOUNCE_BUTTON_TIME = 2000;
int DEBOUNCE_TIME = 500;

#ifndef WIFI_SSID
#define WIFI_SSID "(WIFI_SSID not defined)"
#endif
#ifndef WIFI_PASSWORD
#define WIFI_PASSWORD "(WIFI_PASSWORD not defined)"
#endif

WiFiClient client;
ShellyDimmer* shellyDimmerService = new ShellyDimmer("http://192.168.1.99", true);


void connectWifi(){
  Serial.print("Connection to ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}


boolean isActive = false;
bool previousIsActive = false;
void setup() {
  Serial.begin(115200);
  Serial.println(APP_NAME);
  Serial.println(APP_VERSION);
  connectWifi();
  
  pinMode(IS_ON_LED, OUTPUT);
  pinMode(TURN_ON_SWITCH, INPUT);
  pinMode(BRIGHTNESS_POT, INPUT);  
  isActive = shellyDimmerService->getStatus();
  previousIsActive = isActive;
  digitalWrite(IS_ON_LED, isActive ? HIGH : LOW);
}

int brightness = 0;
int previousBrightness = 0;
unsigned long debounce = 0;
unsigned long debounceRequest = 0;
bool changeDetected = false;
void loop() {
  
  brightness = analogRead(BRIGHTNESS_POT);
  brightness = map(brightness, 0, 1023, 0, 100);
  brightness = brightness < 2 ? 0 : brightness;


  if(debounce + DEBOUNCE_BUTTON_TIME < millis()){
    int res = digitalRead(TURN_ON_SWITCH);
    if(res == HIGH){
      Serial.print("Bouton pressed!");
      debounce = millis();
      isActive = !isActive;
      digitalWrite(IS_ON_LED, isActive ? HIGH : LOW);
    }
  }
  if(abs(brightness-previousBrightness) > 5 || previousIsActive != isActive){
      //Update was made. Start debounce before sending request
      Serial.println("Changes detected !");
      Serial.print("Brightness:");
      Serial.println(brightness);
      debounceRequest = millis();
      previousBrightness = brightness;
      previousIsActive = isActive;
      changeDetected = true;
  }
  if(changeDetected && (debounceRequest + DEBOUNCE_TIME < millis())){
    if(brightness == 0)isActive = false;
    shellyDimmerService->sendAction(isActive, brightness);  
    changeDetected = false;
  }
  delay(100);
}
