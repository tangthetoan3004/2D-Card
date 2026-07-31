#pragma once
#include <QtWidgets/QMainWindow>
#include <QPushButton>
#include <QBoxLayout>
#include <QFileDialog>
#include <QResizeEvent>
#include "ui_MainWindow.h"
#include "viewport/Viewport.h"

//Được khai báo trước mỗi khi tạo 1 project Qt Widget, khai báo rằng lớp giao
// diện sẽ được sinh ra từ file .ui và được đặt trong namespace Ui. Khi đó, lớp MainWindow
// sẽ có thể truy cập các thành phần giao diện được định nghĩa trong file .ui thông qua con trỏ ui.
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindowClass; };
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);
	~MainWindow();

	void SetMenubar();
	void SetToolbar();
	void SetSidebarWidget(QWidget* widget);
	void SetUnderbarWidget(QWidget* widget);

	void resizeEvent(QResizeEvent* event) override;

private:
	Ui::MainWindowClass* ui;
	Viewport* mViewport;
	QWidget* mSidebar;
	QWidget* mUnderbar;
	QAction* mUndoAction;
	QAction* mRedoAction;
};
