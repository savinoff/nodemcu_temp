#include "DHT.h"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <ESP8266HTTPClient.h>

// DHT22 Setup
#define DHTPIN 4     // what digital pin we're connected to. D2 on nodemcu
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);

//v2.0 added blink lightdiode

// WiFi Setup
char ssid[] = "MGTS_GPON_6B33_xxx";
char pass[] = "xxxxxxxxx";

// Blynk Setup
char auth[] = "Xxxxxxxxxxxxxxxx";

void setup() {
  Serial.begin(9600);
  Serial.println("DHT22 blynk");
  dht.begin();
  Blynk.begin(auth, ssid, pass);
  pinMode(2, OUTPUT);
  Serial.println(WiFi.localIP());
}

void loop() {
  delay(500);
  digitalWrite(2, LOW);
  delay(500);
  digitalWrite(2, HIGH);
  delay(1000*60);
  // DHT22 get values
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print("\n");

  Blynk.run();

  Blynk.virtualWrite(V0, t);
  Blynk.virtualWrite(V1, h);

  //Http send 
  HTTPClient http;
  String address = "http://xxxx.xxx.ru/addsensorvalue/";

  //temp
  String sensor = "HomeTemp";
  String value_str = "?value=" + String(t);
  String Link = address + sensor + value_str;
  http.begin(Link);
  int httpCode = http.GET();
  String payload = http.getString();
  Serial.println(httpCode);
  Serial.println(payload);

  //hum
  sensor = "HomeHum";
  value_str = "?value=" + String(h);
  Link = address + sensor + value_str;
  http.begin(Link);
  httpCode = http.GET();
  payload = http.getString();
  Serial.println(httpCode);
  Serial.println(payload);

  http.end();  
}
