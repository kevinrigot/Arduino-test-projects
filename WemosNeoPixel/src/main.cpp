#include <Arduino.h>
// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// Released under the GPLv3 license to match the rest of the
// Adafruit NeoPixel library

#include <Adafruit_NeoPixel.h>
#include <JeVe_EasyOTA.h>

#define ARDUINO_HOSTNAME "ota-neopixel"
#ifndef WIFI_SSID
#define WIFI_SSID "(WIFI_SSID not defined)"
#endif
#ifndef WIFI_PASSWORD
#define WIFI_PASSWORD "(WIFI_PASSWORD not defined)"
#endif
EasyOTA OTA(ARDUINO_HOSTNAME);


#define PIN        D6 
#define RED        D1 
#define GREEN      D2 
#define BLUE       D3 
Adafruit_NeoPixel pixels(1, PIN, NEO_GRB + NEO_KHZ800);

int getValue(int pin){
  int value = 0;
  digitalWrite(pin, HIGH); 
  value = map(analogRead(A0), 0, 1023, 0, 255);
  if(value < 5) value = 0;
  Serial.println(value);
  digitalWrite(pin, LOW);
  delay(10);
  return value;
}

void setup() {
  Serial.begin(9600);
  OTA.onMessage([](const String &message, int line) {
    Serial.println(message);
  });
  // Add networks you wish to connect to
  OTA.addAP(WIFI_SSID, WIFI_PASSWORD);
  // Allow open networks.
  // NOTE: gives priority to configured networks
  OTA.allowOpen(false);

  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(A0, INPUT);

  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels.clear(); // Set all pixel colors to 'off'
}
int red, green, blue = 0;
void loop()
{
  OTA.loop();

  red = getValue(RED);
  green = getValue(GREEN);
  blue = getValue(BLUE);

  pixels.setPixelColor(0, pixels.Color(red, green, blue));

  pixels.show(); 

  delay(1); 
}

