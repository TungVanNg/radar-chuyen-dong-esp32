#include <Arduino.h>
#include <ESP32Servo.h>

#define PIN_SG90 13

Servo sg90;

void setup() {
  Serial.begin(9600);
  sg90.setPeriodHertz(50); // PWM 50Hz
  sg90.attach(PIN_SG90, 1000, 2000); // Chuẩn xung servo
  delay(1000);
}

void loop() {
  // Quay phải xa hơn (tăng delay thêm 150ms)
  sg90.write(130); // Quay phải
  delay(1850);     // Thêm độ
  sg90.write(90);  // Dừng
  delay(500);

  // Quay trái xa hơn
  sg90.write(50);  // Quay trái
  delay(1850);     // Thêm độ
  sg90.write(90);  // Dừng
  delay(500);
}
