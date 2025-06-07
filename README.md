✍️ Tác giả
Tên: NGUYỄN VĂN TÙNG

Trường: Học viện Công nghệ Bưu chính Viễn thông (PTIT)

Năm học: Năm nhất – Ngành Kỹ thuật điện tử viễn thông

# 🚨 Dự án IoT: Radar phát hiện chuyển động bằng ESP32 + HC-SR04

## 📝 Mô tả dự án
Dự án này sử dụng **ESP32** kết hợp với cảm biến siêu âm **HC-SR04** để:
- 📏 Đo khoảng cách từ cảm biến đến vật thể phía trước
- 🏃‍♂️ Phát hiện vật thể chuyển động bằng cách so sánh sự thay đổi khoảng cách
- 📡 Gửi dữ liệu qua WiFi, hiển thị trực tiếp trên **giao diện web**
- 🔐 Hướng tới ứng dụng giám sát an ninh, đếm người, theo dõi tốc độ di chuyển

---

## 🔧 Phần cứng sử dụng
| Thiết bị              | Mô tả                            |
|------------------------|----------------------------------|
| ESP32 DevKit V1        | Vi điều khiển chính               |
| HC-SR04                | Cảm biến siêu âm đo khoảng cách  |
| Breadboard + dây cắm   | Kết nối mạch                     |
| Nguồn 5V cho ESP32     | Có thể dùng cáp microUSB         |
| Laptop + trình duyệt   | Xem dữ liệu qua WiFi             |

---

## 💻 Phần mềm sử dụng
- Arduino IDE (có cài thêm board ESP32)
- Thư viện WiFi (đã tích hợp sẵn)
- Serial Monitor để xem địa chỉ IP
- Trình duyệt bất kỳ để mở giao diện radar

---

## 📸 Hình ảnh minh họa

![Sơ đồ nối dây](images/mach-noi-day.jpg)

> Bạn có thể vẽ sơ đồ bằng Fritzing hoặc chụp hình thực tế

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
