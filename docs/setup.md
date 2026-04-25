---
noteId: "2d5e57c1408d11f1b2c8fbd01491fdd3"
tags: []

---

# Setup Guide

## Requirements

- ESP32 development board
- PlatformIO IDE or Arduino IDE
- ThingSpeak account and Write API Key
- WiFi network
- Smart irrigation PCB or equivalent wiring

## Cloud Gateway Setup

1. Open `firmware/cloud_gateway/cloud_gateway.ino`.
2. Copy the example config file:

```bash
cp firmware/cloud_gateway/config.example.h firmware/cloud_gateway/config.h
```

3. Edit `config.h` and add your WiFi and ThingSpeak credentials.
4. Upload the sketch to the ESP32 cloud gateway board.
5. Open the serial monitor at `115200` baud.

## HMI Display Setup

1. Open `firmware/hmi_display/ESP_CYD_DHT_Master_iteration4_SIMPLE_V2.ino`.
2. Install the required display, touch, LVGL, and TFT libraries.
3. Select the correct ESP32 display board.
4. Upload the sketch.

## Master Controller Setup

1. Open `firmware/master_controller/master_controller.ino`.
2. Upload to the controller board.
3. Confirm I2C communication through the serial monitor.
