---
noteId: "2d5e09a0408d11f1b2c8fbd01491fdd3"
tags: []

---

# System Architecture

## Overview

The Smart Irrigation System monitors environmental and soil conditions, sends data between controllers using I2C, and publishes readings to ThingSpeak for remote monitoring.

## Data Flow

```text
Sensors -> Master Controller -> I2C -> Cloud Gateway -> WiFi -> ThingSpeak Dashboard
                         |
                         v
                    HMI Display
```

## Main Components

- **Master Controller:** receives and processes local sensor/control data.
- **Cloud Gateway:** receives data over I2C and uploads values to ThingSpeak.
- **HMI Display:** provides a graphical interface for monitoring and interaction.
- **ThingSpeak:** stores and visualizes irrigation data.

## ThingSpeak Fields

| Field | Data |
|---|---|
| Field 1 | Soil/moisture sensor value |
| Field 2 | Light/level value |
| Field 3 | Temperature |
| Field 4 | Humidity |
| Field 5 | Potentiometer/threshold value |
| Field 6 | Relay 1 state |
| Field 7 | Relay 2 state |
