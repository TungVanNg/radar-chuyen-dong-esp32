#include <Arduino.h>
#include <ESP32Servo.h>

#define PIN_SG90 13

Servo sg90;

void setup() {
  Serial.begin(9600);
  sg90.setPeriodHertz(50);
  sg90.attach(PIN_SG90, 1000, 2000); // Xung chuẩn 0°–180°
  delay(1000);

  sg90.write(45); // Về vị trí bắt đầu hành trình
  delay(1000);
}

void loop() {
  // Quét từ 45° đến 135° với tốc độ chậm hơn
  for (int pos = 45; pos <= 135; pos++) {
    sg90.write(pos);
    delay(80);  // tăng từ 50 → 80ms (quay chậm hơn)
  }

  // Quét ngược lại từ 135° về 45°
  for (int pos = 135; pos >= 45; pos--) {
    sg90.write(pos);
    delay(80);  // tương tự để đồng bộ tốc độ
  }

  delay(1000); // nghỉ dài hơn trước lần tiếp theo (cho cảm giác nhịp radar)
}
