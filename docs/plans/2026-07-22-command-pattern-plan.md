# Kế hoạch Triển khai: Tích hợp Command Pattern & Undo/Redo cho ứng dụng CAD 2D

> **Yêu cầu đối với AI Agent:** Sử dụng kỹ năng `subagent-driven-development` hoặc `executing-plans` để thực thi kế hoạch này theo từng bước. Sử dụng cú pháp checkbox (`- [ ]`) để theo dõi tiến độ.

**Mục tiêu:** Xây dựng hệ thống lệnh (Command Pattern) thuần C++ cho ứng dụng `simple-2d-cad`, hỗ trợ Undo/Redo cho các hành động vẽ (Line, Face), xóa thực thể (Delete Shape) và tích hợp các nút bấm/phím tắt tương ứng trên giao diện Qt.

**Kiến trúc:** Thiết kế lớp cơ sở trừu tượng `Command` và lớp quản lý `CommandHistory` với hai ngăn xếp `mUndoStack` và `mRedoStack` lưu giữ `std::unique_ptr<Command>`. Các trạng thái tương tác (`State`) và `Viewport` sẽ khởi tạo các đối tượng Command cụ thể (`DrawLineCommand`, `DrawFaceCommand`, `DeleteShapeCommand`) để thực thi và quản lý bộ nhớ an toàn.

**Tech Stack:** C++17/C++20, Qt 6.6.0 (MSVC 2019/2022), MSBuild (`simple-2d-cad.vcxproj`).

---

## Bản đồ Cấu trúc File (File Structure Map)

*   **Tạo mới:**
    *   `command/Command.h`: Lớp cơ sở trừu tượng cho tất cả các lệnh.
    *   `command/CommandHistory.h` & `command/CommandHistory.cpp`: Quản lý danh sách lịch sử lệnh Undo/Redo.
    *   `command/DrawLineCommand.h` & `command/DrawLineCommand.cpp`: Lệnh vẽ đoạn thẳng.
    *   `command/DrawFaceCommand.h` & `command/DrawFaceCommand.cpp`: Lệnh vẽ đa giác.
    *   `command/DeleteShapeCommand.h` & `command/DeleteShapeCommand.cpp`: Lệnh xóa thực thể khỏi Scene.
*   **Chỉnh sửa:**
    *   `viewport/Viewport.h` & `viewport/Viewport.cpp`: Thêm `CommandHistory`, các phương thức `Undo()`, `Redo()`, `DeleteSelected()`.
    *   `state/DrawLineState.cpp` & `state/DrawFaceState.cpp`: Đẩy lệnh vào `CommandHistory` thay vì thêm trực tiếp vào Scene.
    *   `MainWindow.h` & `MainWindow.cpp`: Thêm nút bấm Undo/Redo trên Toolbar và phím tắt `Ctrl+Z`, `Ctrl+Y`.
    *   `simple-2d-cad.vcxproj` & `simple-2d-cad.vcxproj.filters`: Cập nhật cấu hình build dự án Visual Studio.

---

## Danh sách Nhiệm vụ (Tasks)

### Task 1: Xây dựng Lớp Cơ sở `Command` và Lớp Quản lý `CommandHistory`

**Các file liên quan:**
- Tạo mới: `command/Command.h`
- Tạo mới: `command/CommandHistory.h`
- Tạo mới: `command/CommandHistory.cpp`

- [ ] **Bước 1: Tạo header `command/Command.h`**
  Mã nguồn triển khai:
  ```cpp
  #pragma once

  class Command
  {
  public:
      virtual ~Command() = default;
      virtual void Execute() = 0;
      virtual void Undo() = 0;
  };
  ```

- [ ] **Bước 2: Tạo header `command/CommandHistory.h`**
  Mã nguồn triển khai:
  ```cpp
  #pragma once
  #include <vector>
  #include <memory>
  #include "Command.h"

  class CommandHistory
  {
  public:
      CommandHistory() = default;
      ~CommandHistory() = default;

      void PushCommand(std::unique_ptr<Command> cmd);
      bool Undo();
      bool Redo();
      void Clear();

      bool CanUndo() const;
      bool CanRedo() const;

  private:
      std::vector<std::unique_ptr<Command>> mUndoStack;
      std::vector<std::unique_ptr<Command>> mRedoStack;
  };
  ```

- [ ] **Bước 3: Tạo source `command/CommandHistory.cpp`**
  Mã nguồn triển khai:
  ```cpp
  #include "CommandHistory.h"

  void CommandHistory::PushCommand(std::unique_ptr<Command> cmd)
  {
      if (!cmd) return;
      cmd->Execute();
      mUndoStack.push_back(std::move(cmd));
      mRedoStack.clear(); // Xóa sạch Redo stack khi có lệnh mới
  }

  bool CommandHistory::Undo()
  {
      if (mUndoStack.empty()) return false;

      std::unique_ptr<Command> cmd = std::move(mUndoStack.back());
      mUndoStack.pop_back();

      cmd->Undo();
      mRedoStack.push_back(std::move(cmd));
      return true;
  }

  bool CommandHistory::Redo()
  {
      if (mRedoStack.empty()) return false;

      std::unique_ptr<Command> cmd = std::move(mRedoStack.back());
      mRedoStack.pop_back();

      cmd->Execute();
      mUndoStack.push_back(std::move(cmd));
      return true;
  }

  void CommandHistory::Clear()
  {
      mUndoStack.clear();
      mRedoStack.clear();
  }

  bool CommandHistory::CanUndo() const
  {
      return !mUndoStack.empty();
  }

  bool CommandHistory::CanRedo() const
  {
      return !mRedoStack.empty();
  }
  ```

- [ ] **Bước 4: Commit Git**
  Lệnh chạy:
  ```bash
  git add command/Command.h command/CommandHistory.h command/CommandHistory.cpp
  git commit -m "feat(command): tạo lớp cơ sở Command và CommandHistory"
  ```

---

### Task 2: Triển khai các Lệnh cụ thể `DrawLineCommand` và `DrawFaceCommand`

**Các file liên quan:**
- Tạo mới: `command/DrawLineCommand.h` & `command/DrawLineCommand.cpp`
- Tạo mới: `command/DrawFaceCommand.h` & `command/DrawFaceCommand.cpp`

- [ ] **Bước 1: Tạo `command/DrawLineCommand.h` và `command/DrawLineCommand.cpp`**
  Mã nguồn header `DrawLineCommand.h`:
  ```cpp
  #pragma once
  #include "Command.h"
  #include "../viewport/Scene.h"
  #include "../shape/Shape.h"
  #include <vector>

  class DrawLineCommand : public Command
  {
  public:
      DrawLineCommand(Scene* scene, Line* line, const std::vector<Vertex*>& vertices);
      ~DrawLineCommand() override = default;

      void Execute() override;
      void Undo() override;

  private:
      Scene* mScene;
      Line* mLine;
      std::vector<Vertex*> mVertices;
  };
  ```
  Mã nguồn cpp `DrawLineCommand.cpp`:
  ```cpp
  #include "DrawLineCommand.h"

  DrawLineCommand::DrawLineCommand(Scene* scene, Line* line, const std::vector<Vertex*>& vertices)
      : mScene(scene), mLine(line), mVertices(vertices)
  {
  }

  void DrawLineCommand::Execute()
  {
      if (!mScene) return;
      std::list<Shape*> shapes = mScene->retShapes();
      for (auto* v : mVertices)
      {
          shapes.push_back(v);
      }
      if (mLine) shapes.push_back(mLine);
      mScene->UpdateShapes(shapes);
  }

  void DrawLineCommand::Undo()
  {
      if (!mScene) return;
      std::list<Shape*> shapes = mScene->retShapes();
      if (mLine) shapes.remove(mLine);
      for (auto* v : mVertices)
      {
          shapes.remove(v);
      }
      mScene->UpdateShapes(shapes);
  }
  ```

- [ ] **Bước 2: Tạo `command/DrawFaceCommand.h` và `command/DrawFaceCommand.cpp`**
  Mã nguồn header `DrawFaceCommand.h`:
  ```cpp
  #pragma once
  #include "Command.h"
  #include "../viewport/Scene.h"
  #include "../shape/Shape.h"
  #include <vector>

  class DrawFaceCommand : public Command
  {
  public:
      DrawFaceCommand(Scene* scene, Face* face, const std::vector<Vertex*>& vertices);
      ~DrawFaceCommand() override = default;

      void Execute() override;
      void Undo() override;

  private:
      Scene* mScene;
      Face* mFace;
      std::vector<Vertex*> mVertices;
  };
  ```
  Mã nguồn cpp `DrawFaceCommand.cpp`:
  ```cpp
  #include "DrawFaceCommand.h"

  DrawFaceCommand::DrawFaceCommand(Scene* scene, Face* face, const std::vector<Vertex*>& vertices)
      : mScene(scene), mFace(face), mVertices(vertices)
  {
  }

  void DrawFaceCommand::Execute()
  {
      if (!mScene) return;
      std::list<Shape*> shapes = mScene->retShapes();
      for (auto* v : mVertices)
      {
          shapes.push_back(v);
      }
      if (mFace) shapes.push_back(mFace);
      mScene->UpdateShapes(shapes);
  }

  void DrawFaceCommand::Undo()
  {
      if (!mScene) return;
      std::list<Shape*> shapes = mScene->retShapes();
      if (mFace) shapes.remove(mFace);
      for (auto* v : mVertices)
      {
          shapes.remove(v);
      }
      mScene->UpdateShapes(shapes);
  }
  ```

- [ ] **Bước 3: Commit Git**
  Lệnh chạy:
  ```bash
  git add command/DrawLineCommand.* command/DrawFaceCommand.*
  git commit -m "feat(command): tạo các lệnh cụ thể DrawLineCommand và DrawFaceCommand"
  ```

---

### Task 3: Triển khai Lệnh Xóa Thực thể `DeleteShapeCommand` với Quản lý Bộ nhớ An toàn

**Các file liên quan:**
- Tạo mới: `command/DeleteShapeCommand.h` & `command/DeleteShapeCommand.cpp`

- [ ] **Bước 1: Tạo `command/DeleteShapeCommand.h`**
  Mã nguồn triển khai:
  ```cpp
  #pragma once
  #include "Command.h"
  #include "../viewport/Scene.h"
  #include "../shape/Shape.h"
  #include <vector>

  class DeleteShapeCommand : public Command
  {
  public:
      DeleteShapeCommand(Scene* scene, const std::vector<Shape*>& targetShapes);
      ~DeleteShapeCommand() override;

      void Execute() override;
      void Undo() override;

  private:
      Scene* mScene;
      std::vector<Shape*> mDeletedShapes;
      bool mIsDeletedFromScene;
  };
  ```

- [ ] **Bước 2: Tạo `command/DeleteShapeCommand.cpp`**
  Mã nguồn triển khai:
  ```cpp
  #include "DeleteShapeCommand.h"

  DeleteShapeCommand::DeleteShapeCommand(Scene* scene, const std::vector<Shape*>& targetShapes)
      : mScene(scene), mDeletedShapes(targetShapes), mIsDeletedFromScene(false)
  {
  }

  DeleteShapeCommand::~DeleteShapeCommand()
  {
      // Nếu các hình đang nằm ngoài Scene (đã bị xóa) khi lệnh này bị giải phóng, thực hiện delete bộ nhớ
      if (mIsDeletedFromScene)
      {
          for (Shape* s : mDeletedShapes)
          {
              delete s;
          }
          mDeletedShapes.clear();
      }
  }

  void DeleteShapeCommand::Execute()
  {
      if (!mScene || mDeletedShapes.empty()) return;

      std::list<Shape*> shapes = mScene->retShapes();
      for (Shape* s : mDeletedShapes)
      {
          shapes.remove(s);
      }
      mScene->UpdateShapes(shapes);
      mIsDeletedFromScene = true;
  }

  void DeleteShapeCommand::Undo()
  {
      if (!mScene || mDeletedShapes.empty()) return;

      std::list<Shape*> shapes = mScene->retShapes();
      for (Shape* s : mDeletedShapes)
      {
          shapes.push_back(s);
      }
      mScene->UpdateShapes(shapes);
      mIsDeletedFromScene = false;
  }
  ```

- [ ] **Bước 3: Commit Git**
  Lệnh chạy:
  ```bash
  git add command/DeleteShapeCommand.h command/DeleteShapeCommand.cpp
  git commit -m "feat(command): tạo DeleteShapeCommand hỗ trợ quản lý bộ nhớ an toàn"
  ```

---

### Task 4: Tích hợp Command History vào `Viewport` và các Trạng thái (`State`)

**Các file liên quan:**
- Chỉnh sửa: `viewport/Viewport.h`
- Chỉnh sửa: `viewport/Viewport.cpp`
- Chỉnh sửa: `state/DrawLineState.cpp`
- Chỉnh sửa: `state/DrawFaceState.cpp`

- [ ] **Bước 1: Cập nhật `Viewport.h` để thêm `CommandHistory` và các hàm điều khiển**
  Thêm thành phần `CommandHistory* mHistory` và các phương thức `Undo()`, `Redo()`, `PushCommand()`, `DeleteSelectedShape()` vào `Viewport.h`.

- [ ] **Bước 2: Triển khai các phương thức Undo/Redo/Delete trong `Viewport.cpp`**
  Cập nhật hàm khởi tạo `Viewport::Viewport` để tạo `mHistory = new CommandHistory()`.
  Lắng nghe phím `Delete` trong `Viewport::keyPressEvent` để thực hiện xóa các đối tượng đang được chọn.

- [ ] **Bước 3: Cập nhật `DrawLineState.cpp` và `DrawFaceState.cpp`**
  Thay vì đẩy trực tiếp hình vào `Scene`, sử dụng `mViewport->PushCommand(...)` tạo đối tượng `DrawLineCommand` và `DrawFaceCommand`.

- [ ] **Bước 4: Commit Git**
  Lệnh chạy:
  ```bash
  git add viewport/Viewport.* state/DrawLineState.cpp state/DrawFaceState.cpp
  git commit -m "feat(viewport): tích hợp CommandHistory và xử lý lệnh Undo/Redo/Delete"
  ```

---

### Task 5: Tích hợp Nút bấm Undo/Redo & Phím tắt vào Giao diện `MainWindow`

**Các file liên quan:**
- Chỉnh sửa: `MainWindow.h`
- Chỉnh sửa: `MainWindow.cpp`

- [ ] **Bước 1: Cập nhật `MainWindow.h`**
  Thêm các QAction con trỏ `mUndoAction`, `mRedoAction` và phương thức `UpdateUndoRedoActions()`.

- [ ] **Bước 2: Cập nhật `MainWindow.cpp`**
  Trong `SetToolbar()`, thêm nút Undo và Redo với phím tắt `QKeySequence::Undo` (`Ctrl+Z`) và `QKeySequence::Redo` (`Ctrl+Y`).
  Lập trình slot kết nối tới `mViewport->Undo()` và `mViewport->Redo()`.

- [ ] **Bước 3: Commit Git**
  Lệnh chạy:
  ```bash
  git add MainWindow.h MainWindow.cpp
  git commit -m "feat(ui): thêm nút Undo/Redo và phím tắt Ctrl+Z, Ctrl+Y trên MainWindow toolbar"
  ```

---

### Task 6: Cập nhật Tệp Cấu hình Dự án MSBuild (`simple-2d-cad.vcxproj`)

**Các file liên quan:**
- Chỉnh sửa: `simple-2d-cad.vcxproj`
- Chỉnh sửa: `simple-2d-cad.vcxproj.filters`

- [ ] **Bước 1: Thêm các tệp `command/*.h` và `command/*.cpp` vào project file**
  Thêm các mục `<ClInclude Include="command\*.h" />` và `<ClCompile Include="command\*.cpp" />` để Visual Studio nhận diện các file mới khi build solution.

- [ ] **Bước 2: Commit Git**
  Lệnh chạy:
  ```bash
  git add simple-2d-cad.vcxproj simple-2d-cad.vcxproj.filters
  git commit -m "chore(build): thêm các file command module vào Visual Studio vcxproj"
  ```
