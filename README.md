✍️ Tác giả
Tên: NGUYỄN VĂN TÙNG

# 🚨 Dự án IoT: Radar phát hiện chuyển động bằng ESP32 + HC-SR04 + SERVO 360 + LED + BUZZ

## 📝 Mô tả dự án
Dự án này sử dụng **ESP32** kết hợp với cảm biến siêu âm **HC-SR04** để:
- 📏 Đo khoảng cách từ cảm biến đến vật thể phía trước
- 🏃‍♂️ Phát hiện vật thể chuyển động bằng cách so sánh sự thay đổi khoảng cách
- 📡 Gửi dữ liệu qua WiFi, hiển thị trực tiếp trên **giao diện web**
- 🔐 Hướng tới ứng dụng giám sát an ninh, đếm người, theo dõi tốc độ di chuyển

---

## 🔧 Phần cứng sử dụng
| Thiết bị / Linh kiện                 | Số lượng | Ghi chú                                                  |
| ------------------------------------ | -------- | -------------------------------------------------------- |
| ESP32 DevKit V1                      | 1        | Bo mạch chính                                            |
| Servo 360 độ (SG90 hoặc tương đương) | 1        | Gắn cảm biến có thể quay                                 |
| Cảm biến siêu âm HC-SR04             | 1        | Đo khoảng cách                                           |
| Breadboard                           | 1        | Kết nối không cần hàn, dây jumper cái (\~10 dây)         |
| Buzzer 5V                            | 1        | Cảnh báo phát hiện vật gần                               |
| LED đỏ                               | 1        | Hiển thị cảnh báo                                        |
| Điện trở 220Ω                        | 1        | Dùng với LED                                             |
| Cáp microUSB                         | 1        | Nguồn và lập trình cho ESP32                             |
| Máy tính xách tay                    | 1        | Sử dụng Arduino IDE                                      |
| Nguồn 5V ngoài                       | 1        | Nếu thiết lập lại servo gây ra lỗi, nên dùng nguồn riêng |


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
| Thiết bị   | Chân ESP32                           | Ghi chú                                               |
| ---------- | ------------------------------------ | ----------------------------------------------------- |
| Servo 360° | GND, VCC 5V, GPIO 13                 | GND nối GND, VCC nối 5V, Signal điều khiển từ GPIO 13 |
| HC-SR04    | VCC, GND, Trig, Echo                 | Trig: GPIO 5<br>Echo: GPIO 18                         |
| Buzzer     | (+): GPIO 27, (–): GND               | Cảnh báo khi có vật thể gần                           |
| LED đỏ     | (+): GPIO 26, (–): GND, qua trở 220Ω | Sáng khi phát hiện vật gần                            |

## 💻 Phần mềm sử dụng
- Arduino IDE (có cài thêm board ESP32)
- Thư viện WiFi (đã tích hợp sẵn)
- Serial Monitor để xem địa chỉ IP
- Trình duyệt bất kỳ để mở giao diện radar

---

## 📸 Hình ảnh minh họa
![mach](https://github.com/user-attachments/assets/cc1beb67-11ff-4ae0-bba4-0391685bc183)


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
