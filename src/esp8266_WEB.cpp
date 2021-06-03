#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <Hash.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <EEPROM.h>

/* functions declaration */
String processor(const String& var);
void setup();
void loop();

/* --------------------- */


// Replace with your network credentials
const char* ssid = "TERRA";
const char* password = "1234567890";
/* Local SSID */
const char* ssid_soft_ap     = "TERMOSTATO";
const char* password_soft_ap = NULL;          // setting NULL for no password
int Wifi_SoftAP_Channel      = 5;
// ESP 8266 Local Network Configuration
IPAddress local_IP(10,10,10,10);
IPAddress gateway(10,10,10,10);
IPAddress subnet(255,255,255,0);

#define DHTPIN D1     // Digital pin connected to the DHT sensor
#define RELAISPIN D3  // Digital pin connected to relais
#define WIFIPIN 16    // Digital pin connected to secondary builtin LED

// Uncomment the type of sensor in use:
//#define DHTTYPE    DHT11     // DHT 11
#define DHTTYPE    DHT22     // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)

DHT dht(DHTPIN, DHTTYPE);

// current temperature & humidity, updated in loop()
float t = 0.0;
float h = 0.0;
float tempdes = 27.0;
float tolerance = 0.05;
float incr = 0.5;
float offset = 0.50;
boolean st = false;
boolean wificonnected = false;
uint addr = 0; // for the EEPROM

  struct { 
    uint val;
    float tempread;
  } data;
// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;    // will store last time DHT was updated

// Updates DHT readings every 10 seconds
const long interval = 5000;  

#include "webserver.html"

// Replaces placeholder with DHT values
String processor(const String& var){
  //Serial.println(var);
  if(var == "TEMPERATURE"){
    return String(t);
  }
  else if(var == "HUMIDITY"){
    return String(h);
  }
  else if(var == "TEMPDESIRED"){
    return String(tempdes);
  }
  else if(var == "STATUS"){
    if(st==false) {
      return String("SPENTO");
    } else {
      return String("ACCESO");
    }
  }
  else if (var == "WIFI") {
    if (wificonnected == true) {
      return WiFi.localIP().toString();
    } else {
      return String("Not Connected!");
    }
  }
  return String();
}

void setup(){
  int maxretry = 60;  // try to connect to wifi for 1 minute
  

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(WIFIPIN, OUTPUT);
  pinMode(RELAISPIN, OUTPUT);

  digitalWrite(RELAISPIN, LOW); // turn off relais

  EEPROM.begin(512);
  EEPROM.get(addr,data);
  if (data.val != 26) {
    data.val = 26;
    data.tempread = tempdes;
    EEPROM.put(addr, data);
    EEPROM.commit();
  }
  tempdes = data.tempread;
   

  // Serial port for debugging purposes
  Serial.begin(115200);
  dht.begin();

  Serial.println("Connecting to WiFi");


  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  digitalWrite(WIFIPIN, HIGH);
  digitalWrite(LED_BUILTIN, HIGH);
  
  WiFi.softAPConfig(local_IP, gateway, subnet);
  WiFi.softAP(ssid_soft_ap, password_soft_ap, Wifi_SoftAP_Channel);

  while (WiFi.status() != WL_CONNECTED) {
    delay(900);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
    digitalWrite(LED_BUILTIN, HIGH);
    maxretry--;
    if (maxretry <= 0) {
      break;
    }
  }
  if (maxretry > 0) {
    digitalWrite(WIFIPIN, LOW); // turn on WifiPIN
    Serial.println(WiFi.localIP());
    wificonnected = true;
  }
  // Print ESP8266 Local IP Address

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    String resp = String(t);
    resp += String(";");
    if(st==false) {
      resp += String("SPENTO");
    } else {
      resp += String("ACCESO");
    }
    request->send_P(200, "text/plain", resp.c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", String(h).c_str());
  });
  server.on("/tempup", HTTP_GET, [](AsyncWebServerRequest *request){
    tempdes += incr;
    request->send_P(200, "text/plain", String(tempdes).c_str());
  });
  server.on("/tempdn", HTTP_GET, [](AsyncWebServerRequest *request){
    tempdes -= incr;
    request->send_P(200, "text/plain", String(tempdes).c_str());
  });

  server.on("/save", HTTP_GET, [](AsyncWebServerRequest *request){
    data.val = 26;
    data.tempread = tempdes;
    EEPROM.put(addr, data);
    EEPROM.commit();
    request->send_P(200, "text/plain", String("Salvato").c_str());
  });



  // Start server
  server.begin();
}
 
void loop(){  
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // save the last time you updated the DHT values
    previousMillis = currentMillis;
    // Read temperature as Celsius (the default)
    float newT = dht.readTemperature();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    //float newT = dht.readTemperature(true);
    // if temperature read failed, don't change t value
    if (isnan(newT)) {
      Serial.println("Failed to read from DHT sensor!");
    }
    else {
      t = newT  + offset;
      if (t > (tempdes + tolerance)) {
        st = false;
      }
      if (t < (tempdes - tolerance)) {
        st = true;
      }
      if (st == true) {
        digitalWrite(LED_BUILTIN, LOW);
        digitalWrite(RELAISPIN, HIGH); // turn on relais
      } else {
        digitalWrite(LED_BUILTIN, HIGH);
        digitalWrite(RELAISPIN, LOW); // turn off relais
      }
      // Serial.println(t);
    }
    // Read Humidity
    float newH = dht.readHumidity();
    // if humidity read failed, don't change h value 
    if (isnan(newH)) {
      Serial.println("Failed to read from DHT sensor!");
    }
    else {
      h = newH;
      // Serial.println(h);
    }
  }
}
