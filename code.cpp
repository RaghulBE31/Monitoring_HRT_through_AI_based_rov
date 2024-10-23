#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <esp_camera.h>
#include <HTTPClient.h>

// Replace with your network credentials
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

// Motor control pins
const int motor1_forward = 23;  // Adjust according to your wiring
const int motor1_backward = 22;
const int motor2_forward = 25;
const int motor2_backward = 26;

// Create a web server
AsyncWebServer server(80);

// Function to stop the motors
void stopMotors() {
    digitalWrite(motor1_forward, LOW);
    digitalWrite(motor1_backward, LOW);
    digitalWrite(motor2_forward, LOW);
    digitalWrite(motor2_backward, LOW);
}

// Function to move forward
void moveForward() {
    digitalWrite(motor1_forward, HIGH);
    digitalWrite(motor1_backward, LOW);
    digitalWrite(motor2_forward, HIGH);
    digitalWrite(motor2_backward, LOW);
}

// Function to move backward
void moveBackward() {
    digitalWrite(motor1_forward, LOW);
    digitalWrite(motor1_backward, HIGH);
    digitalWrite(motor2_forward, LOW);
    digitalWrite(motor2_backward, HIGH);
}

// Function to turn left
void turnLeft() {
    digitalWrite(motor1_forward, LOW);
    digitalWrite(motor1_backward, HIGH);
    digitalWrite(motor2_forward, HIGH);
    digitalWrite(motor2_backward, LOW);
}

// Function to turn right
void turnRight() {
    digitalWrite(motor1_forward, HIGH);
    digitalWrite(motor1_backward, LOW);
    digitalWrite(motor2_forward, LOW);
    digitalWrite(motor2_backward, HIGH);
}

// Handle command from Raspberry Pi
void handleCommand(AsyncWebServerRequest *request) {
    String command = request->getParam("cmd")->value();

    if (command == "forward") {
        moveForward();
    } else if (command == "backward") {
        moveBackward();
    } else if (command == "left") {
        turnLeft();
    } else if (command == "right") {
        turnRight();
    } else if (command == "stop") {
        stopMotors();
    }

    request->send(200, "text/plain", "Command executed: " + command);
}

// Function to start the camera
void startCamera() {
    camera_config_t config;
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = 5;   // Adjust according to your wiring
    config.pin_d1 = 18;  // Adjust according to your wiring
    config.pin_d2 = 19;  // Adjust according to your wiring
    config.pin_d3 = 21;  // Adjust according to your wiring
    config.pin_d4 = 36;  // Adjust according to your wiring
    config.pin_d5 = 39;  // Adjust according to your wiring
    config.pin_d6 = 34;  // Adjust according to your wiring
    config.pin_d7 = 35;  // Adjust according to your wiring
    config.pin_xclk = 0;  // Adjust according to your wiring
    config.pin_pclk = 22; // Adjust according to your wiring
    config.pin_vsync = 25;// Adjust according to your wiring
    config.pin_href = 23; // Adjust according to your wiring
    config.pin_sscb_sda = 26; // Adjust according to your wiring
    config.pin_sscb_scl = 27; // Adjust according to your wiring
    config.pin_pwdn = -1; // Power down pin
    config.pin_reset = -1; // Software reset pin

    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG;
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12; // 0-63 lower number means higher quality
    config.fb_count = 2; // If more than one, faster but may use more RAM

    // Initialize the camera
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        Serial.printf("Camera init failed with error 0x%x", err);
        return;
    }
}

// Function to capture and send image
void sendImageToRaspberryPi() {
    camera_fb_t *fb = esp_camera_fb_get();
    if (!fb) {
        Serial.println("Camera capture failed");
        return;
    }

    // Send image to Raspberry Pi
    HTTPClient http;
    http.begin("http://<raspberry_pi_ip>/upload_image"); // Replace with your Raspberry Pi IP
    http.addHeader("Content-Type", "image/jpeg");
    int httpResponseCode = http.POST(fb->buf, fb->len);
    
    if (httpResponseCode > 0) {
        Serial.printf("Image sent successfully, response code: %d\n", httpResponseCode);
    } else {
        Serial.printf("Failed to send image, error: %s\n", http.errorToString(httpResponseCode).c_str());
    }

    esp_camera_fb_return(fb);
    http.end();
}

// Function to send sensor data to Raspberry Pi
void sendSensorData(float distance) {
    HTTPClient http;
    http.begin("http://<raspberry_pi_ip>/sensor_data"); // Replace with your Raspberry Pi IP
    http.addHeader("Content-Type", "application/json");
    
    // Prepare JSON data
    String jsonData = "{\"distance\": " + String(distance) + "}";
    int httpResponseCode = http.POST(jsonData);

    if (httpResponseCode > 0) {
        Serial.printf("Sensor data sent successfully, response code: %d\n", httpResponseCode);
    } else {
        Serial.printf("Failed to send sensor data, error: %s\n", http.errorToString(httpResponseCode).c_str());
    }

    http.end();
}

// Setup function
void setup() {
    Serial.begin(115200);
    pinMode(motor1_forward, OUTPUT);
    pinMode(motor1_backward, OUTPUT);
    pinMode(motor2_forward, OUTPUT);
    pinMode(motor2_backward, OUTPUT);
    
    // Connect to Wi-Fi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("Connected to WiFi");
    Serial.println("IP address: " + WiFi.localIP().toString());

    // Start camera
    startCamera();

    // Define routes
    server.on("/command", HTTP_GET, handleCommand);
    server.begin();
}

void loop() {
    // Send image and sensor data periodically
    static unsigned long lastSendTime = 0;
    unsigned long currentMillis = millis();
    
    if (currentMillis - lastSendTime >= 5000) { // Every 5 seconds
        float distance = random(0, 100); // Simulate distance measurement (replace with actual sensor code)
        sendImageToRaspberryPi();
        sendSensorData(distance);
        lastSendTime = currentMillis;
    }
}
