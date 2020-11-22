// This version only supports a single Plant.
#include "WiFi.h"
#include "SPIFFS.h"
#include "ESPAsyncWebServer.h"

//#define ssid "DavidsZimmerpflanze"
//#define pwd "VerySecureYesYes"


#define value_len 144

#define second_delay 600

int sensor_pin = 33;
int sensor_value = 0;

float values[value_len];
float timeline[value_len];
String timeline_str;

AsyncWebServer server(80);

void setup() {
  pinMode(sensor_pin, INPUT);
  //Init Data Array
  for (int i = 0; i<value_len; i++) {
    //values[i] = (float(i)/float(value_len))*(float(i)/float(value_len));
    values[i] = 0;
  }


  //Create Timeline

  //Newest Right
  for (int i = value_len-1; i>=0; i--) {
    timeline[i] = 0 - float((value_len-i)*second_delay)/60;
  }

  //Newest Left
  //for (int i = 0; i<value_len; i++) {
  //  timeline[i] = 0 - i*second_delay;
  //}

  timeline_str = timeline_to_json();

  Serial.begin(115200);

  //Setup WiFi
  //WiFi.softAP(ssid, pwd);
  //Serial.println(WiFi.softAPIP());

  Serial.print("Connecting to WiFi..");

  WiFi.begin(ssid, pwd);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println(WiFi.localIP());

  ////Mount File System
  if(!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  server.on(
   "/dashboard", HTTP_GET,
   [](AsyncWebServerRequest *request){
     request->send(SPIFFS, "/index.html", "text/html");
   }
  );

  server.on(
    "/data", HTTP_GET,
    [](AsyncWebServerRequest *request){
      request->send(200, "text/json", values_to_json());
    }
  );

  server.on(
    "/timeline", HTTP_GET,
    [](AsyncWebServerRequest *request){
      request->send(200, "text/json", timeline_str);
    }
  );

  server.begin();

}

void push_value(float val) {
  //for(int i = value_len-1; i>=0; i--) {
  //  values[i] = values[i-1]; //Shift values right
  //}

  //values[0] = val;

  for(int i = 0; i<value_len-1; i++) {
    values[i] = values[i+1]; //Shift values left
  }

  values[value_len-1] = val;

  Serial.println(val);
}

String values_to_json() {
  String json = "[\n";
  for (int i = 0; i<value_len; i++) {
    json += "\t";
    json += String(values[i]);
    if (i != value_len-1) {
     json += ",\n";
    }
  }

  json += "\n]";
  return json;
}

String timeline_to_json() {
    String json = "[\n";
  for (int i = 0; i<value_len; i++) {
    json += "\t\"";
    json += String(timeline[i]);
    json += "\"";
    if (i != value_len-1) {
     json += ",\n";
    }
  }

  json += "\n]";
  return json;
}

void loop() {
  sensor_value = analogRead(sensor_pin);
  Serial.println(sensor_value);
  push_value(float(sensor_value)/2047.0);

  if( WiFi.status() != WL_CONNECTED ){
    //Try to recconect if connection is lost....
    WiFi.disconnect();
    WiFi.begin(ssid, pwd);
  }
  
  delay(second_delay*1000);
}
