// Code demo đọc HC-SR04, phát hiện chuyển động đơn giản

#include <WiFi.h>

// WiFi thông tin (thay thành của bạn)
const char* ssid = "Ten_WiFi";
const char* password = "MatKhauWiFi";

// HC-SR04 chân
const int trigPin = 5;  // GPIO5
const int echoPin = 18; // GPIO18

long duration;
int distance;
int prevDistance = 0;
bool motionDetected = false;

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Kết nối WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  distance = readDistance();
  motionDetected = abs(distance - prevDistance) > 10;
  prevDistance = distance;

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm - Motion: ");
  Serial.println(motionDetected ? "YES" : "NO");

  // Phục vụ client web
  WiFiClient client = server.available();
  if (client) {
    String response = "<html><head><title>Radar IoT</title></head><body>";
    response += "<h1>Radar ESP32 + HC-SR04</h1>";
    response += "<p>Distance: " + String(distance) + " cm</p>";
    response += "<p>Motion detected: " + String(motionDetected ? "YES" : "NO") + "</p>";
    response += "</body></html>";

    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println("Connection: close");
    client.println();
    client.println(response);
    delay(10);
    client.stop();
  }

  delay(1000);
}

int readDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  int dist = duration * 0.034 / 2;
  return dist;
}
