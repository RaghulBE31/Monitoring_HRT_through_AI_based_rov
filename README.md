# Monitering_HRT_through_AI_based_rov


## Project Overview

This project involves the development of an AI-Based Remote Operated Vehicle (ROV) that utilizes an ESP32-CAM for real-time monitoring and data collection. The system captures images and sensor data, sending them to a Raspberry Pi, which hosts a trained AI model for further processing. The vehicle can be controlled remotely via HTTP commands.

### Key Features

- Real-time image capturing and streaming using ESP32-CAM.
- Motor control for driving the ROV forward, backward, left, and right.
- Sensor data collection and transmission (e.g., distance measurements).
- A Flask web server running on Raspberry Pi to receive and process images and sensor data.
- Integration with AI models for additional functionalities such as crack detection.

## Hardware Requirements

- **ESP32-CAM**
- **Raspberry Pi (any model with Wi-Fi capability)**
- Motor driver (L298N or similar)
- DC motors and wheels
- Power supply (battery or USB)
- Jumper wires and a breadboard
- Distance sensor (optional, for enhanced functionality)

## Software Requirements

- **ESP32 Libraries:**
  - `WiFi.h`
  - `ESPAsyncWebServer.h`
  - `esp_camera.h`
- **Python Libraries:**
  - Flask (`pip install Flask`)

## Setup Instructions

### ESP32-CAM Configuration

1. **Install the ESP32 Board in Arduino IDE:**
   - Go to File > Preferences, and add the following URL to the "Additional Board Manager URLs":
     ```
     https://dl.espressif.com/dl/package_esp32_index.json
     ```
   - Then, go to Tools > Board > Boards Manager, search for "ESP32" and install it.

2. **Upload the ESP32-CAM Code:**
   - Open the provided ESP32-CAM code in the Arduino IDE.
   - Update the `ssid` and `password` variables with your Wi-Fi credentials.
   - Replace `<raspberry_pi_ip>` with the actual IP address of your Raspberry Pi.
   - Upload the code to your ESP32-CAM.

### Raspberry Pi Configuration

1. **Install Flask:**
   ```bash
   pip install Flask
