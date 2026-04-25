#include <Wire.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define SLAVE_ADDR 0x08

// ================= WIFI =================
#include "config.h"


// ================= THINGSPEAK =================
String apiKey = THINGSPEAK_WRITE_API_KEY;

// ================= DATA =================
byte buffer[18];

int16_t s = 0, L = 0, pot = 0, relay1 = 0, relay2 = 0;
float t = 0.0, h = 0.0;

volatile bool newData = false;

// ================= I2C RECEIVE =================
void receiveEvent(int bytes)
{
  if (bytes != 18) {
    while (Wire.available()) Wire.read();
    return;
  }

  for (int i = 0; i < 18; i++) {
    buffer[i] = Wire.read();
  }

  memcpy(&s, buffer, 2);
  memcpy(&L, buffer + 2, 2);
  memcpy(&t, buffer + 4, 4);
  memcpy(&h, buffer + 8, 4);
  memcpy(&pot, buffer + 12, 2);
  memcpy(&relay1, buffer + 14, 2);
  memcpy(&relay2, buffer + 16, 2);

  Serial.println("---- RECEIVED ----");
  Serial.print("S: "); Serial.println(s);
  Serial.print("L: "); Serial.println(L);
  Serial.print("T: "); Serial.println(t);
  Serial.print("H: "); Serial.println(h);
  Serial.print("POT: "); Serial.println(pot);
  Serial.print("RELAY1: "); Serial.println(relay1);
  Serial.print("RELAY2: "); Serial.println(relay2);

  newData = true;
}

// ================= SEND TO THINGSPEAK =================
void sendToThingSpeak()
{
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected");
    return;
  }

  HTTPClient http;

  String url = "http://api.thingspeak.com/update?api_key=" + apiKey +
               "&field1=" + String(s) +
               "&field2=" + String(L) +
               "&field3=" + String(t, 2) +
               "&field4=" + String(h, 2) +
               "&field5=" + String(pot) +
               "&field6=" + String(relay1) +
               "&field7=" + String(relay2);

  http.begin(url);

  int httpCode = http.GET();

  Serial.print("ThingSpeak response: ");
  Serial.println(httpCode);

  http.end();
}

// ================= SETUP =================
void setup()
{
  Serial.begin(115200);
  delay(2000);

  // I2C SLAVE
  Wire.begin(SLAVE_ADDR);
  Wire.onReceive(receiveEvent);

  // WIFI CONNECT
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected!");
  Serial.println(WiFi.localIP());

  Serial.println("ESP32 READY");
}

// ================= LOOP =================
void loop()
{
  static unsigned long lastSend = 0;

  // Reconnect WiFi if needed
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Reconnecting WiFi...");
    WiFi.reconnect();
    delay(2000);
    return;
  }

  // Send data every 15 seconds (ThingSpeak limit)
  if (newData && millis() - lastSend > 15000) {
    Serial.println("Sending to ThingSpeak...");
    
    sendToThingSpeak();

    newData = false;
    lastSend = millis();
  }

  delay(10);
}