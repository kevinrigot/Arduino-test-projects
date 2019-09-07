Components:
- NodeMcu
- FPM10A

https://randomnerdtutorials.com/fingerprint-sensor-module-with-arduino/

Notes:
- It works only between 3.3-4.2V! If the voltage drops, it crash.
- It often crash (returning communication error). Solution: Reset the fingerprint sensor (disconnect-reconnect)
- A small delay has been added from the example. It solved an issue that the board could not detect the sensor

How to use:
1. Enroll some fingerprint. In src, you should have only 1 cpp. main_enroll.cpp
2. Test your fingerprint. In src, you should have only 1 cpp. main_test.cpp