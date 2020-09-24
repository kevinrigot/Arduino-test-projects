#include <Arduino.h>
#include <RTCVars.h>

RTCVars state; // create the state object

int reset_counter;                      // we want to keep these values after reset
int program_step;

void setup() {
  Serial.begin(115200);                 // allow debug output
  
  state.registerVar( &reset_counter );  // we send a pointer to each of our variables
  state.registerVar( &program_step );

  if (state.loadFromRTC()) {            // we load the values from rtc memory back into the registered variables
    reset_counter++;
    Serial.println("This is reset no. " + (String)reset_counter);
    state.saveToRTC();                  // since we changed a state relevant variable, we store the new values
  } else {
    reset_counter = 0;                  // cold boot part
    Serial.println("This seems to be a cold boot. We don't have a valid state on RTC memory");
    program_step = 0;
    state.saveToRTC();                    // there are no parameters because it only needs the vars' definiton once 
  }

  Serial.println("Going into deep sleep for 5 seconds");
  ESP.deepSleep(5e6);
}

void loop() {
}
