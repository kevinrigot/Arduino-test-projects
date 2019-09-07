#include <Arduino.h>

#include <JeVe_EasyOTA.h>

#define ARDUINO_HOSTNAME "ota-flash-demo"
#ifndef WIFI_SSID
#define WIFI_SSID "(WIFI_SSID not defined)"
#endif
#ifndef WIFI_PASSWORD
#define WIFI_PASSWORD "(WIFI_PASSWORD not defined)"
#endif
#define ARDUINO_HOSTNAME "ota-flash-demo"
EasyOTA OTA(ARDUINO_HOSTNAME);

//variabls for blinking an LED with Millis
unsigned long previousMillis = 0;  // will store last time LED was updated
const long interval = 2000;  // interval at which to blink (milliseconds)
int ledState = LOW;  // ledState used to set the LED

void setup()
{
  Serial.begin(9600);
  // This callback will be called when EasyOTA has anything to tell you.
  OTA.onMessage([](const String &message, int line) {
    Serial.println(message);
  });
  // Add networks you wish to connect to
  OTA.addAP(WIFI_SSID, WIFI_PASSWORD);
  // Allow open networks.
  // NOTE: gives priority to configured networks
  OTA.allowOpen(false);

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  OTA.loop();
    //loop to blink without delay
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    // if the LED is off turn it on and vice-versa:
    ledState = not(ledState);
    // set the LED with the ledState of the variable:
    digitalWrite(LED_BUILTIN,  ledState);
  }
}