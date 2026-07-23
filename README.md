# 2D-Card - High-Performance 2D CAD Application in C++ & Qt

[![C++ Standard](https://img.shields.io/badge/C%2B%2B-17%2F20-blue.svg)](https://en.cppreference.com/)
[![Qt Framework](https://img.shields.io/badge/Qt-6.6.0-green.svg)](https://www.qt.io/)
[![License](https://img.shields.io/badge/License-MIT-purple.svg)](LICENSE)

**2D-Card** là một ứng dụng đồ họa kỹ thuật CAD 2D được thiết kế và phát triển bằng ngôn ngữ **C++** và **Qt 6 Framework**. Ứng dụng mô phỏng các tính năng cốt lõi của phần mềm CAD chuyên nghiệp (như AutoCAD / VinaCAD), áp dụng nghiêm ngặt các mẫu thiết kế hướng đối tượng nâng cao (**Command Pattern**, **State Pattern**, **Model-View Architecture**) cùng cấu trúc bộ nhớ an toàn với con trỏ thông minh C++ (`std::unique_ptr`, `std::shared_ptr`).

---

## 🌟 Các Tính Năng Nâng Cao (PRO Features)

### 📐 1. Phép Biến Đổi Hình Học 2D (2D Affine Transformations)
*   **Move (Di chuyển):** Dịch chuyển thực thể theo vector vị trí $(\Delta x, \Delta y)$ với điểm mốc (Base Point). Phím tắt `M`.
*   **Copy (Sao chép):** Nhân bản đối tượng chính xác và tự động gán vào Scene. Phím tắt `C`.
*   **Rotate (Phép xoay):** Xoay thực thể quanh điểm mốc theo ma trận xoay góc $\theta$. Phím tắt `R`.
*   **Scale (Co giãn):** Ph phóng to / thu nhỏ hình theo hệ số tỷ lệ $k$.
*   **Xem trước Trực quan (Live Preview):** Hỗ trợ nét đứt xem trước vị trí biến đổi theo thời gian thực (Real-time Dashed Guide Line).

### 🎯 2. Hệ Thống Bắt Điểm Thông Minh (Advanced OSNAP - Object Snap)
*   **Endpoint (Điểm mút - $\blacksquare$):** Bắt chính xác các đỉnh của đoạn thẳng, đa giác (Marker hình vuông xanh lá).
*   **Midpoint (Trung điểm - $\Delta$):** Bắt chính xác điểm chính giữa của đoạn thẳng $M = \frac{V_1 + V_2}{2}$ (Marker hình tam giác xanh lam).
*   **Intersection (Giao điểm - $\times$):** Giải hệ phương trình tính giao điểm cắt nhau giữa 2 đoạn thẳng (Marker dấu X màu cam).
*   **Perpendicular (Chân đường vuông góc - $\perp$):** Tính toán chân đường vuông góc từ con trỏ chuột tới đoạn thẳng (Marker biểu tượng vuông góc màu tím).
*   **Công tắc OSNAP (Phím F3):** Bật/Tắt chế độ bắt điểm linh hoạt thông qua nút bấm Toolbar hoặc phím tắt `F3`.

### 🎨 3. Hệ Thống Quản Lý Lớp Đồ Họa (Layer Management System)
*   **Quản lý Lớp (Layer Properties):** Thiết lập Tên (Name), Màu sắc (Color), Trạng thái Ẩn/Hiện (Show/Hide) và Khóa/Mở khóa (Lock/Free).
*   **Lớp Mặc định (`Layer 0`) & Lớp Hiện tại (`Active Layer`):** Các đối tượng mới tạo tự động gán theo Active Layer.
*   **Lọc Hiển thị (Render Filtering):** Ẩn các hình thuộc Layer đang bị tắt mà không xóa dữ liệu khỏi bộ nhớ.
*   **Bảo vệ Khóa (Lock Protection):** Ngăn chặn việc xóa hoặc biến đổi các thực thể thuộc Layer bị khóa.
*   **Sidebar Panel (`LayerWidget`):** Bảng tương tác quản lý Layer trực tiếp bên hông giao diện chính, tích hợp hộp thoại chọn màu `QColorDialog`.

### 🔄 4. Kiến Trúc Command Pattern & Undo/Redo
*   **Undo (`Ctrl + Z`) / Redo (`Ctrl + Y`):** Quản lý hoàn tác và làm lại không giới hạn cấp độ cho các thao tác Tạo mới, Xóa, Di chuyển, Sao chép, Xoay và Co giãn.
*   **Memory Safety:** Sử dụng `std::unique_ptr<Command>` và quản lý bộ nhớ C++ an toàn, chống rò rỉ bộ nhớ (Zero Memory Leaks).

### 💾 5. Quản Lý Sơ Đồ & Lưu Trữ (JSON Serialization)
*   Hỗ trợ Tạo mới Scene (`New`), Lưu (`Save`) và Đọc (`Open`) toàn bộ sơ đồ dưới dạng định dạng dữ liệu mã hóa **JSON**.

---

## 🛠️ Yêu Cầu & Môi Trường Phát Triển (Prerequisites)

| Công cụ / Thư viện | Phiên bản khuyến nghị |
| :--- | :--- |
| **Ngôn ngữ** | C++17 / C++20 |
| **Framework** | Qt 6.6.0 (MSVC 2019/2022 64-bit) |
| **IDE** | Visual Studio 2022 |
| **Build Tool** | MSBuild / Visual Studio Solution (`.sln`) |
| **Hệ điều hành** | Windows 10 / 11 |

---

## 🏗️ Cấu Trúc Mã Nguồn (Project Structure)

```text
simple-2d-cad/
├── command/              # Bộ các lớp Command Pattern (Draw, Delete, Move, Copy, Rotate, Scale)
├── layer/                # Cấu trúc Layer, LayerManager và LayerWidget Sidebar Panel
├── shape/                # Định nghĩa các đối tượng hình học (Vertex, Line, Face)
├── state/                # Bộ các lớp State Pattern (DrawLineState, DrawFaceState, TransformState)
├── utils/                # Thuật toán hình học SelectUtils, OSNAP math & Snap Markers
├── viewport/             # Thành phần hiển thị đồ họa Viewport, Scene, Camera
├── docs/                 # Tài liệu thiết kế (specs) và kế hoạch triển khai (plans)
├── MainWindow.cpp/.h     # Cửa sổ chính ứng dụng CAD, thanh công cụ Toolbar & Sidebar
└── main.cpp              # Điểm khởi chạy chương trình (Entry point)
```

---

## 🚀 Hướng Dẫn Biên Dịch Và Chạy (Build & Run)

1. **Clone repository về máy:**
   ```bash
   git clone git@github.com:tangthetoan3004/2D-Card.git
   ```
2. **Mở dự án trên Visual Studio 2022:**
   - Mở file `simple-2d-cad.sln`.
   - Đảm bảo đã cài đặt Extension **Qt Visual Studio Tools** và đã trỏ đúng đường dẫn `Qt 6.6.0 MSVC64`.
3. **Biên dịch và Chạy:**
   - Chọn cấu hình `Debug` hoặc `Release` (x64).
   - Nhấn **F5** (hoặc `Ctrl + F5`) để biên dịch và trải nghiệm ứng dụng CAD.
