#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

class ShellyDimmer{
    private:
        String host;
        String statusUrl;
        String actionUrl;
        boolean debug;
    public:
        ShellyDimmer(String host):
            host(host){
                statusUrl = host + "/status";
                actionUrl = host + "/light/0";
        }
        ShellyDimmer(String host, boolean debug):
            host(host), debug(debug){
                statusUrl = host + "/status";
                actionUrl = host + "/light/0";
        }
        boolean getStatus(){
            Serial.print("Requesting URL: ");
            Serial.println(statusUrl);
            if(debug)return true;
            HTTPClient http;
            WiFiClient client;
            http.begin(client, statusUrl);
            int httpCode = http.GET();
            
            DynamicJsonDocument doc(2048);
            bool lightIsOn = false;
            if (httpCode > 0) { 
                DeserializationError error = deserializeJson(doc, http.getStream());   
                if (error) {
                    Serial.print(F("deserializeJson() failed: "));
                    Serial.println(error.f_str());     
                }else{
                    lightIsOn = doc["lights"][0]["ison"];
                    Serial.print("Light is currently ");
                    Serial.println(lightIsOn ? "on" : "off");
                }    
            }else{
                Serial.print("Error! Command not accepted. Error code: ");
                Serial.println(httpCode); 
            }

            http.end();
            return lightIsOn;
        }

        void sendAction(bool turnOn, int brightness){
            String fullUrl = actionUrl + "?turn=" + (turnOn ? "on" : "off")+ "&brightness="+brightness;
            Serial.print("Requesting URL: ");
            Serial.println(fullUrl);
            if(debug)return;
            HTTPClient http;
            WiFiClient client;
            http.begin(client, fullUrl);
            int httpCode = http.GET();
            
            if (httpCode == 200) {     
                Serial.println("Command accepted");
            }else{
                Serial.print("Error! Command not accepted. Error code: ");
                Serial.println(httpCode);
            }
            http.end();
        }
};
