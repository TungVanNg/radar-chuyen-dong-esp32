✍️ Tác giả
Tên: NGUYỄN VĂN TÙNG

Trường: Học viện Công nghệ Bưu chính Viễn thông (PTIT)

Năm học: Năm nhất – Ngành Kỹ thuật điện tử viễn thông

# 🚨 Dự án IoT: Radar phát hiện chuyển động bằng ESP32 + HC-SR04 + SERVO 360 + LED + BUZZ

## 📝 Mô tả dự án
Dự án này sử dụng **ESP32** kết hợp với cảm biến siêu âm **HC-SR04** để:
- 📏 Đo khoảng cách từ cảm biến đến vật thể phía trước
- 🏃‍♂️ Phát hiện vật thể chuyển động bằng cách so sánh sự thay đổi khoảng cách
- 📡 Gửi dữ liệu qua WiFi, hiển thị trực tiếp trên **giao diện web**
- 🔐 Hướng tới ứng dụng giám sát an ninh, đếm người, theo dõi tốc độ di chuyển

---

## 🔧 Phần cứng sử dụng
Thiết bị	Số lượng	Ghi chú
ESP32 DevKit V1	1	Bo mạch chính
Servo 360 độ (SG90 hoặc tương đương)	1	Gắn cảm biến để quay
Cảm biến siêu âm HC-SR04	1	Đo khoảng cách
Breadboard	1	Kết nối không cần hàn
Dây jumper đực-cái	~10	Đủ để nối các chân
Buzzer 5V	1	Cảnh báo phát hiện vật thể gần
LED đỏ	1	Hiển thị cảnh báo
Điện trở 220Ω	1	Nối tiếp LED
Cáp microUSB	1	Cấp nguồn và nạp code
Laptop	1	Dùng Arduino IDE và mở trình duyệt
(Tùy chọn) Nguồn 5V ngoài	1	Nếu servo gây reset, nên dùng riêng

---
🔌 Sơ đồ nối dây phần cứng
Thiết bị	Chân	Nối tới GPIO trên ESP32	Ghi chú
Servo 360°	GND	GND	GND chung với ESP32
VCC	5V	Nếu ESP32 yếu, nên dùng nguồn ngoài
Signal	GPIO 13	Dùng PWM để quay
HC-SR04	VCC	5V	Cấp nguồn cảm biến
GND	GND	
Trig	GPIO 5	Gửi xung
Echo	GPIO 18	Nhận xung, nên dùng phân áp nếu cẩn thận
Buzzer	(+)	GPIO 27	Cảnh báo khi có vật thể gần
(–)	GND	
LED	(+) (qua trở 220Ω)	GPIO 26	Sáng khi phát hiện vật cản
(–)	GND

## 💻 Phần mềm sử dụng
- Arduino IDE (có cài thêm board ESP32)
- Thư viện WiFi (đã tích hợp sẵn)
- Serial Monitor để xem địa chỉ IP
- Trình duyệt bất kỳ để mở giao diện radar

---

## 📸 Hình ảnh minh họa

![Sơ đồ nối dây](images/mach-noi-day.jpg)

---

## 🧠 Cách hoạt động
1. ESP32 đọc khoảng cách từ cảm biến HC-SR04 liên tục
2. So sánh với khoảng cách trước đó để phát hiện chuyển động
3. Nếu khoảng cách thay đổi > 10 cm → xem là có chuyển động
4. Dữ liệu sẽ được cập nhật mỗi giây trên giao diện web

---

## 🚀 Cách sử dụng

### Bước 1: Chuẩn bị
- Mở **Arduino IDE**
- Cài board ESP32 theo hướng dẫn: [Link hướng dẫn chi tiết](https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide/)
- Nối dây theo sơ đồ

### Bước 2: Cài đặt WiFi
Trong code, sửa phần này thành WiFi nhà bạn:
```cpp
const char* ssid = "Ten_WiFi";
const char* password = "MatKhauWiFi";
```
Bước 3: Nạp code
Mở file code/radar_esp32.ino

Cắm ESP32, chọn cổng COM phù hợp, chọn board “ESP32 Dev Module”

Nhấn Upload

Bước 4: Mở Serial Monitor
Sau khi nạp xong, mở Serial Monitor sẽ thấy địa chỉ IP ví dụ: 192.168.1.69

Dán IP vào trình duyệt → sẽ thấy radar hoạt động
