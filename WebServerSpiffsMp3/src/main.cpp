#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <FS.h>
#include <AudioFileSourceSPIFFS.h>
#include <AudioFileSourceID3.h>
#include <AudioGeneratorMP3.h>
#include <AudioOutputI2SNoDAC.h>

#include <ArduinoJson.h>

#define ARDUINO_HOSTNAME "ota-webserver-spiff"
#ifndef WIFI_SSID
#define WIFI_SSID "(WIFI_SSID not defined)"
#endif
#ifndef WIFI_PASSWORD
#define WIFI_PASSWORD "(WIFI_PASSWORD not defined)"
#endif

#define led LED_BUILTIN

//***********************************
//************* Gestion du serveur WEB
//***********************************
ESP8266WebServer server(80);
AudioGeneratorMP3 *mp3 = new AudioGeneratorMP3();
AudioFileSourceSPIFFS *file;
AudioOutputI2SNoDAC *out;
AudioFileSourceID3 *id3;

void handleRoot()
{
  server.sendHeader("Location", "/index.html", true); //Redirige vers page index.html sur SPIFFS
  server.send(302, "text/plane", "");
}

uint8_t ledStatus = HIGH;

// fonction de traitement SETLED
void handleLED()
{
  String requestedStatus = server.arg("status");
  Serial.println("Change led status to " + requestedStatus);
  // On NodeMcu, led is on when set to LOW
  if (requestedStatus == "on"){
    ledStatus = LOW;
  }else{
    ledStatus = HIGH;
  }
  digitalWrite(led, ledStatus);
  server.send(200, "text/plain", requestedStatus);
}

// gestion du not found.
bool loadFromSpiffs(String path)
{
  String dataType = "text/plain";
  if (path.endsWith("/"))
    path += "index.htm";

  if (path.endsWith(".src"))
    path = path.substring(0, path.lastIndexOf("."));
  else if (path.endsWith(".html"))
    dataType = "text/html";
  else if (path.endsWith(".htm"))
    dataType = "text/html";
  else if (path.endsWith(".css"))
    dataType = "text/css";
  else if (path.endsWith(".js"))
    dataType = "application/javascript";
  else if (path.endsWith(".png"))
    dataType = "image/png";
  else if (path.endsWith(".gif"))
    dataType = "image/gif";
  else if (path.endsWith(".jpg"))
    dataType = "image/jpeg";
  else if (path.endsWith(".ico"))
    dataType = "image/x-icon";
  else if (path.endsWith(".xml"))
    dataType = "text/xml";
  else if (path.endsWith(".pdf"))
    dataType = "application/pdf";
  else if (path.endsWith(".zip"))
    dataType = "application/zip";
  File dataFile = SPIFFS.open(path.c_str(), "r");
  if (server.hasArg("download"))
    dataType = "application/octet-stream";
  if (server.streamFile(dataFile, dataType) != dataFile.size())
  {
  }

  dataFile.close();
  return true;
}
// Handle page "not found"
void handleWebRequests()
{
  if (loadFromSpiffs(server.uri()))
    return;
  String message = "File Not Detected\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++)
  {
    message += " NAME:" + server.argName(i) + "\n VALUE:" + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  Serial.println(message);
}
// Called when a metadata event occurs (i.e. an ID3 tag, an ICY block, etc.
void MDCallback(void *cbData, const char *type, bool isUnicode, const char *string)
{
  (void)cbData;
  Serial.printf("ID3 callback for: %s = '", type);

  if (isUnicode)
  {
    string += 2;
  }

  while (*string)
  {
    char a = *(string++);
    if (isUnicode)
    {
      string++;
    }
    Serial.printf("%c", a);
  }
  Serial.printf("'\n");
  Serial.flush();
}
String soundPlaying = "";
void playSound()
{
  if (!mp3->isRunning())
  {
    String sound = server.arg("sound");
    String soundToPlay = "/pno-cs.mp3";
    if(sound && sound != ""){
      soundToPlay = "/" + sound;
    }
    char* s = (char*)soundToPlay.c_str();
    file = new AudioFileSourceSPIFFS(s);
    id3 = new AudioFileSourceID3(file);
    id3->RegisterMetadataCB(MDCallback, (void *)"ID3TAG");
    out = new AudioOutputI2SNoDAC();
    mp3 = new AudioGeneratorMP3();
    bool result = mp3->begin(id3, out);
    if(result){
      Serial.println("Sample MP3 playback begins...\n");
      soundPlaying = soundToPlay;
      server.send(200, "text/plain", soundToPlay);
    }else{
      server.send(400, "text/plain", "Error loading " + soundToPlay);
    }
    
  }
}

StaticJsonDocument<JSON_OBJECT_SIZE(80)> doc;
char output[128];
void getStatus(){
  doc["ledIsOn"] = ledStatus == LOW;
  doc["sound"] = soundPlaying;
  serializeJson(doc, output, sizeof(output));
  server.send(200, "application/json", output);
}

void setup()
{

  Serial.begin(115200);
  Serial.println();

  SPIFFS.begin();
  Serial.println("Start file System");

  // Connection WIFI
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connection ok on network :  ");
  Serial.println(WIFI_SSID);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  //Initialize Webserver
  server.on("/", handleRoot);
  server.on("/status", getStatus); 
  server.on("/led", handleLED);      
  server.on("/play", playSound);        
  server.onNotFound(handleWebRequests); //Pour page not found - redirige vers index.html
  server.begin();

  pinMode(led, OUTPUT);
  digitalWrite(led, ledStatus);
}

void loop(){
  server.handleClient();
  if (soundPlaying != ""){
    if (mp3->isRunning()){
      if (!mp3->loop())
        mp3->stop();
    }else{
      Serial.println("MP3 done");
      soundPlaying = "";
    }
  }
}
