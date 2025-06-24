#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <ESP32Servo.h>
#include "html.h"  // Include file HTML header

// WiFi credentials
const char* ssid = "Homie 10/5";
const char* password = "motdentam";

// Pin definitions
#define PIN_SG90 13
#define TRIG_PIN 5
#define ECHO_PIN 18
#define BUZZER_PIN 27
#define LED_PIN 26

// Objects
Servo sg90;
WebServer server(80);

// Variables
int currentAngle = 0;
int distance = 0;
bool isMovingRight = true;
unsigned long servoStartTime = 0;
unsigned long lastMeasurement = 0;
unsigned long lastBuzzTime = 0;
const unsigned long measureInterval = 100; // Đo mỗi 100ms
const unsigned long buzzInterval = 300;    // Buzzer kêu mỗi 300ms khi phát hiện vật

// Servo timing - Thay đổi để quay 360 độ
enum ServoState {
  MOVING_0_TO_180,
  PAUSE_AT_180,
  MOVING_180_TO_360,
  PAUSE_AT_360,
  MOVING_360_TO_180,
  PAUSE_AT_180_BACK,
  MOVING_180_TO_0,
  PAUSE_AT_0
};

ServoState servoState = MOVING_0_TO_180;
unsigned long stateStartTime = 0;
bool objectDetected = false;

void setup() {
  Serial.begin(115200); // Tăng baud rate để debug tốt hơn
  
  // Initialize pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  
  // Turn off buzzer and LED initially
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
  
  // Initialize servo
  sg90.setPeriodHertz(50);
  sg90.attach(PIN_SG90, 1000, 2000);
  
  // Startup sequence
  startupSequence();
  
  // Connect to WiFi with timeout
  Serial.println("Connecting to WiFi...");
  Serial.print("SSID: ");
  Serial.println(ssid);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println();
    Serial.println("WiFi connected successfully!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.print("Signal strength: ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
    
    // Báo hiệu kết nối thành công
    for (int i = 0; i < 5; i++) {
      digitalWrite(LED_PIN, HIGH);
      delay(100);
      digitalWrite(LED_PIN, LOW);
      delay(100);
    }
  } else {
    Serial.println();
    Serial.println("WiFi connection failed!");
    Serial.println("Continuing without WiFi...");
    
    // Báo hiệu lỗi kết nối
    for (int i = 0; i < 3; i++) {
      digitalWrite(BUZZER_PIN, HIGH);
      delay(1000);
      digitalWrite(BUZZER_PIN, LOW);
      delay(500);
    }
  }
  
  // Setup web server routes
  server.on("/", handleRoot);
  server.on("/readDistance", handleReadDistance);
  
  server.begin();
  Serial.println("Web server started");
  
  // Initialize servo state
  sg90.write(130); // Bắt đầu quay phải
  servoState = MOVING_0_TO_180;
  stateStartTime = millis();
  currentAngle = 0;
}

void loop() {
  // Handle WiFi reconnection
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi disconnected. Attempting to reconnect...");
    WiFi.reconnect();
    delay(1000);
  }
  
  server.handleClient();
  
  // Update servo movement
  updateServoMovement();
  
  // Update radar measurement
  if (millis() - lastMeasurement >= measureInterval) {
    updateRadarMeasurement();
    lastMeasurement = millis();
  }
  
  // Handle buzzer timing for object detection
  handleBuzzer();
  
  // Test buzzer every 5 seconds if no object detected
  static unsigned long lastBuzzerTest = 0;
  if (!objectDetected && millis() - lastBuzzerTest > 5000) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(100);
    digitalWrite(BUZZER_PIN, LOW);
    lastBuzzerTest = millis();
    Serial.println("Buzzer test beep");
  }
}

void updateServoMovement() {
  unsigned long currentTime = millis();
  unsigned long elapsedTime = currentTime - stateStartTime;
  
  switch (servoState) {
    case MOVING_0_TO_180: // Quay từ 0° đến 180°
      if (elapsedTime >= 1850) {
        sg90.write(90); // Dừng
        servoState = PAUSE_AT_180;
        stateStartTime = currentTime;
        currentAngle = 180;
      } else {
        currentAngle = map(elapsedTime, 0, 1850, 0, 180);
      }
      break;
      
    case PAUSE_AT_180:
      if (elapsedTime >= 200) { // Dừng ngắn 200ms
        sg90.write(130); // Tiếp tục quay phải
        servoState = MOVING_180_TO_360;
        stateStartTime = currentTime;
      }
      break;
      
    case MOVING_180_TO_360: // Quay từ 180° đến 360°
      if (elapsedTime >= 1850) {
        sg90.write(90); // Dừng
        servoState = PAUSE_AT_360;
        stateStartTime = currentTime;
        currentAngle = 360;
      } else {
        currentAngle = map(elapsedTime, 0, 1850, 180, 360);
      }
      break;
      
    case PAUSE_AT_360:
      if (elapsedTime >= 500) { // Dừng lâu hơn ở 360°
        sg90.write(50); // Quay trái
        servoState = MOVING_360_TO_180;
        stateStartTime = currentTime;
      }
      break;
      
    case MOVING_360_TO_180: // Quay từ 360° về 180°
      if (elapsedTime >= 1850) {
        sg90.write(90); // Dừng
        servoState = PAUSE_AT_180_BACK;
        stateStartTime = currentTime;
        currentAngle = 180;
      } else {
        currentAngle = map(elapsedTime, 0, 1850, 360, 180);
      }
      break;
      
    case PAUSE_AT_180_BACK:
      if (elapsedTime >= 200) { // Dừng ngắn
        sg90.write(50); // Tiếp tục quay trái
        servoState = MOVING_180_TO_0;
        stateStartTime = currentTime;
      }
      break;
      
    case MOVING_180_TO_0: // Quay từ 180° về 0°
      if (elapsedTime >= 1850) {
        sg90.write(90); // Dừng
        servoState = PAUSE_AT_0;
        stateStartTime = currentTime;
        currentAngle = 0;
      } else {
        currentAngle = map(elapsedTime, 0, 1850, 180, 0);
      }
      break;
      
    case PAUSE_AT_0:
      if (elapsedTime >= 500) { // Dừng lâu hơn ở 0°
        sg90.write(130); // Quay phải
        servoState = MOVING_0_TO_180;
        stateStartTime = currentTime;
      }
      break;
  }
}

void updateRadarMeasurement() {
  // Measure distance
  distance = measureDistance();
  
  // Handle object detection - Tăng ngưỡng phát hiện
  if (distance < 50 && distance > 2) { // Tăng từ 40cm lên 50cm
    objectDetected = true;
    digitalWrite(LED_PIN, HIGH);
  } else {
    objectDetected = false;
    digitalWrite(LED_PIN, LOW);
  }
  
  // Debug output mỗi giây
  static unsigned long lastDebug = 0;
  if (millis() - lastDebug > 1000) {
    Serial.print("Angle: ");
    Serial.print(currentAngle);
    Serial.print("°, Distance: ");
    Serial.print(distance);
    Serial.print("cm, State: ");
    Serial.print(servoState);
    Serial.print(", WiFi: ");
    Serial.println(WiFi.status() == WL_CONNECTED ? "Connected" : "Disconnected");
    lastDebug = millis();
  }
}

void handleBuzzer() {
  if (objectDetected) {
    // Tính tần số kêu dựa trên khoảng cách - càng gần càng kêu nhanh
    unsigned long buzzDelay = map(distance, 2, 50, 100, 500); // 2cm->100ms, 50cm->500ms
    
    if (millis() - lastBuzzTime >= buzzDelay) {
      digitalWrite(BUZZER_PIN, HIGH);
      delay(50); // Kêu ngắn 50ms
      digitalWrite(BUZZER_PIN, LOW);
      lastBuzzTime = millis();
      Serial.print("BUZZ! Distance: ");
      Serial.println(distance);
    }
  }
}

int measureDistance() {
  // Clear trigger
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  
  // Send trigger pulse
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  // Read echo with timeout
  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  
  if (duration == 0) {
    return 400; // No echo received
  }
  
  // Calculate distance
  int dist = duration * 0.034 / 2;
  
  // Limit to reasonable range
  if (dist > 400) dist = 400;
  if (dist < 2) dist = 2;
  
  return dist;
}

void startupSequence() {
  Serial.println("Starting radar system...");
  
  // Test all components
  Serial.println("Testing LED...");
  for (int i = 0; i < 3; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(300);
    digitalWrite(LED_PIN, LOW);
    delay(300);
  }
  
  Serial.println("Testing Buzzer...");
  for (int i = 0; i < 3; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(200);
    digitalWrite(BUZZER_PIN, LOW);
    delay(200);
  }
  
  Serial.println("Testing Servo...");
  sg90.write(90);  // Center
  delay(500);
  sg90.write(0);   // Left
  delay(500);
  sg90.write(180); // Right
  delay(500);
  sg90.write(90);  // Center
  delay(500);
  
  Serial.println("Radar system initialized!");
}

void handleRoot() {
  server.send(200, "text/html", MAIN_page);
}

void handleReadDistance() {
  // Create JSON response
  DynamicJsonDocument doc(128);
  doc[0] = distance;
  doc[1] = currentAngle;
  
  String response;
  serializeJson(doc, response);
  
  server.send(200, "application/json", response);
}