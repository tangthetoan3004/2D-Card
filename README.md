# 2D-Card - 2D CAD Application in C++ & Qt

**2D-Card** là một ứng dụng đồ họa kỹ thuật CAD 2D được phát triển bằng ngôn ngữ **C++** và **Qt Framework**. Ứng dụng hỗ trợ vẽ, quản lý các đối tượng hình học 2D (Điểm, Đoạn thẳng, Đa giác khép kín) và tích hợp các mẫu thiết kế hướng đối tượng (Design Patterns) nâng cao như **Command Pattern**, **State Pattern**.

---

## 🌟 Các Tính Năng Chính (Features)

### 🎨 1. Công cụ Vẽ & Hình học (Drawing & Geometry)
- **Vẽ Đoạn thẳng (Draw Line):** Xác định tọa độ thực tế và kết nối 2 đỉnh.
- **Vẽ Đa giác (Draw Face):** Kết nối liên tiếp các đỉnh và khép kín mặt đa giác.

### 🔄 2. Hệ thống Lệnh & Lịch sử (Command Pattern & Undo/Redo)
- **Undo / Redo:** Hỗ trợ hoàn tác (`Ctrl + Z`) và làm lại (`Ctrl + Y`) cho các thao tác vẽ và xóa thực thể.
- **Xóa Thực thể (Delete Shape):** Lựa chọn và xóa đối tượng khỏi Scene bằng phím `Delete`.
- **Quản lý Bộ nhớ An toàn:** Áp dụng con trỏ thông minh (`std::unique_ptr`) và thu hồi bộ nhớ C++ an toàn tuyệt đối.

### 🎯 3. Công cụ Lựa chọn & Snapping (Select & Snapping)
- Chế độ chọn Điểm (Point), Đoạn thẳng (Line), Đa giác (Face).
- Hỗ trợ bắt điểm (Snapping) và kiểm tra va chạm (Hit-testing) dựa trên thuật toán hình học.

### 💾 4. Quản lý File (File Serialization)
- Tạo mới Scene (`New`).
- Lưu (`Save`) và Tải (`Open`) sơ đồ dưới dạng dữ liệu định dạng **JSON**.

---

## 🛠️ Yêu cầu & Môi trường Phát triển (Prerequisites)

| Công cụ / Thư viện | Phiên bản khuyến nghị |
| :--- | :--- |
| **Ngôn ngữ** | C++17 / C++20 |
| **Framework** | Qt 6.6.0 (MSVC 64-bit) |
| **IDE** | Visual Studio 2022 |
| **Hệ điều hành** | Windows 10 / 11 |

---

## 🚀 Hướng dẫn Biên dịch và Chạy (Build & Run)

1. **Cài đặt Visual Studio 2022:** Chọn workload *Desktop development with C++*.
2. **Cài đặt Qt 6.6.0:** Chọn bản MSVC 2019/2022 64-bit.
3. **Cài đặt Extension Qt VS Tools:**
   - Mở Visual Studio $\rightarrow$ `Extensions` $\rightarrow$ `Manage Extensions` $\rightarrow$ Cài đặt **Qt Visual Studio Tools**.
   - Thêm phiên bản Qt trong `Extensions` $\rightarrow$ `Qt VS Tools` $\rightarrow$ `Qt Versions` (Trỏ đường dẫn đến `qmake.exe`).
4. **Clone và Chạy dự án:**
   ```bash
   git clone git@github.com:tangthetoan3004/2D-Card.git
   ```
   - Mở file giải pháp `simple-2d-cad.sln` bằng Visual Studio.
   - Nhấn **F5** để Build và Run ứng dụng.
