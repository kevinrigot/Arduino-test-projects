Test with an ESP8266 to enter Deep sleep and store data in RTC memory and restore it

Components:
- ESP8266 (Wemos D1 Mini)
- Jumper


Important Notes:

When uploading the code, remove the jumper. When running the code put the jumper between D0 (GPIO 16) and RST. 

The RST pin of the ESP8266 is always HIGH while the ESP8266 is running. However, when the RST pin receives a LOW signal, it restarts the microcontroller.

If you set a deep sleep timer with the ESP8266, once the timer ends, GPIO 16 sends a LOW signal. That means that GPIO 16, when connected to the RST pin, can wake up the ESP8266 after a set period of time.


References:
- https://diyi0t.com/how-to-reduce-the-esp8266-power-consumption/
- https://randomnerdtutorials.com/esp8266-deep-sleep-with-arduino-ide/
- https://www.esp8266.com/viewtopic.php?p=83007

Schema:

![Alt text](./schemas/deep_sleep_bb.png?raw=true "connection when running")