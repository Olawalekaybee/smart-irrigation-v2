
#include <Wire.h>
#include <WiFi.h>
#include <HTTPClient.h>
#define SLAVE_ADDR 0x08


byte buffer[12];

void receiveEvent(int bytes) {
  if (bytes != 12) {
    while (Wire.available()) Wire.read();
    return;
  }

  for (int i = 0; i < 12; i++) {
    buffer[i] = Wire.read();
  }

  int16_t s, L;
  float t, h;

  memcpy(&s, buffer, 2);
  memcpy(&L, buffer + 2, 2);
  memcpy(&t, buffer + 4, 4);
  memcpy(&h, buffer + 8, 4);

  Serial.println("---- RECEIVED ----");
  Serial.print("Int A: "); Serial.println(s);
  Serial.print("Int B: "); Serial.println(L);
  Serial.print("Float C: "); Serial.println(t);
  Serial.print("Float D: "); Serial.println(h);
}

void setup() {
  Serial.begin(115200);
  delay(2000);

  Wire.begin(SLAVE_ADDR);
  Wire.onReceive(receiveEvent);

  Serial.println("ESP32 Ready");
}

void loop() {}