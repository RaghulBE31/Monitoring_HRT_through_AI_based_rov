# Monitoring_HRT_through_AI_Based_ROV

## 🚀 Project Overview

This project focuses on developing an **AI-powered Remotely Operated Vehicle (ROV)** for monitoring head race tunnels. The system utilizes an **ESP32-CAM** for live video streaming and a **Raspberry Pi** as the central controller to perform:

- Real-time AI-based crack detection from video frames
- Live dashboard display of camera feed and ML results
- Manual movement control of the ROV via GPIO-based motor driver (L298N)

### 🔧 Core Components

- **ESP32-CAM** streams MJPEG video
- **Raspberry Pi** runs a Flask web server to:
  - Show live video
  - Analyze video frames using a trained ML model
  - Control motors based on user input

---

## 🧠 Key Features

- 📷 Live video feed from ESP32-CAM embedded into the dashboard
- 🧠 Real-time crack detection using TensorFlow (frame-by-frame)
- ⚙️ Motor control (Forward, Backward, Left, Right, Stop) via GPIO
- 🌐 Web interface hosted on Raspberry Pi using Flask
- 🔌 Clean separation of camera (ESP32) and control (Raspberry Pi)

---

## 🛠️ Hardware Requirements

| Component              | Purpose                                |
|------------------------|----------------------------------------|
| **ESP32-CAM**          | Live camera stream                     |
| **Raspberry Pi**       | Central control + AI + web server      |
| water pumps**          | For boyance                            |
| **2 DC Motors**        | Movement of ROV                        |
| **Power Supply**       | Battery or 5V USB for Pi and ESP32     |
| **Jumper Wires**       | Connections to GPIO/motors             |


---

## 💻 Software Stack

### ESP32-CAM (Arduino C++)

- `WiFi.h`
- `ESPAsyncWebServer.h`
- `esp_camera.h`

### Raspberry Pi (Python)

- `Flask`
- `RPi.GPIO`
- `TensorFlow / Keras`
- `OpenCV` *(optional for extended functionality)*

---

## ⚙️ System Architecture
 [ESP32-CAM]
 ↓ (Video Stream: MJPEG @ /stream)
 [Raspberry Pi]
 ↓ Receives images (optional frame capture)
 ↓ Runs AI model for crack detection
 ↓ Web UI (Flask): Live stream + result + control buttons
 ↓ GPIO signals to L298N motor driver
 [ROV Motors]

 
---

## 🔧 Setup Instructions

### 1️⃣ ESP32-CAM Setup

1. **Install ESP32 Board Support in Arduino IDE:**
   - File > Preferences → Add board URL:  
     ```
     https://dl.espressif.com/dl/package_esp32_index.json
     ```
   - Tools > Board > Board Manager → Search `ESP32` and install

2. **Upload the ESP32-CAM Code:**
   - Open your ESP32 sketch
   - Set your `ssid`, `password`, and `raspberry_pi_ip`
   - Upload the code to the board
   - ESP32 will stream video at:  
     ```
     http://<esp32_ip>:81/stream
     ```
     
### 2️⃣ Raspberry Pi Setup

1. **Install Python Requirements:**

```bash
sudo apt update
sudo apt install python3-pip
pip3 install flask tensorflow requests pillow
sudo apt-get install python3-opencv

Run the Flask Server: python3 app.py

Open in browser: http://<raspberry_pi_ip>:5000

