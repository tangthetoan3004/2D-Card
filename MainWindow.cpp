#include "MainWindow.h"
#include "layer/LayerWidget.h"
#include <QDockWidget>


MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindowClass())
{
	// Create main Viewport
	mViewport = new Viewport(this);

	// Set UI
	ui->setupUi(this);
	setMinimumSize(400, 400);
	SetMenubar();
	SetToolbar();
	setCentralWidget(mViewport);

	SetSidebarWidget(mViewport);
	SetUnderbarWidget(mViewport);

	LayerWidget* layerWidget = new LayerWidget(mViewport->GetScene()->GetLayerManager(), mViewport, this);
	QDockWidget* layerDock = new QDockWidget("Layer Manager", this);
	layerDock->setWidget(layerWidget);
	addDockWidget(Qt::RightDockWidgetArea, layerDock);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::SetMenubar()
{
	/* Menu Icon */
	QPixmap newpix("img/new.png");
	QPixmap openpix("img/open.png");
	QPixmap quitpix("img/quit.png");


	/* File */
	QAction* newa = new QAction(newpix, "&New", this);
	QAction* open = new QAction(openpix, "&Open", this);
	QAction* quit = new QAction(quitpix, "&Quit", this);
	quit->setShortcut(tr("ESC"));

	QMenu* file = menuBar()->addMenu("&File");
	file->addAction(newa);
	file->addAction(open);
	file->addSeparator();
	file->addAction(quit);

	qApp->setAttribute(Qt::AA_DontShowIconsInMenus, false);  // Icons are no longer shown in menus

	connect(quit, &QAction::triggered, qApp, QApplication::quit);


	/* Edit */
	QMenu* edit = menuBar()->addMenu("&Edit");
	mUndoAction = edit->addAction("&Undo");
	mUndoAction->setShortcut(QKeySequence::Undo);
	mRedoAction = edit->addAction("&Redo");
	mRedoAction->setShortcut(QKeySequence::Redo);
	connect(mUndoAction, &QAction::triggered, [this]() { mViewport->Undo(); });
	connect(mRedoAction, &QAction::triggered, [this]() { mViewport->Redo(); });


	/* View */
	QAction* viewst = new QAction("&View statusbar", this);
	viewst->setCheckable(true);
	viewst->setChecked(true);

	QMenu* view = menuBar()->addMenu("&View");
	view->addAction(viewst);
	statusBar()->showMessage("Ready");

	if (viewst->isChecked())
	{
		statusBar()->show();
	}
	else
	{
		statusBar()->hide();
	}
}

void MainWindow::SetToolbar()
{
	/* Toolbar */
	QToolBar* toolbar = addToolBar("main toolbar");
	QAction* newa = toolbar->addAction(QIcon(QPixmap("img/new.png")), "New File");
	QAction* open = toolbar->addAction(QIcon(QPixmap("img/open.png")), "Open File");
	QAction* save = toolbar->addAction(QIcon(QPixmap("img/save.png")), "Save File");
	toolbar->addSeparator();

	QAction* undoBtn = toolbar->addAction("Undo");
	undoBtn->setShortcut(QKeySequence::Undo);
	QAction* redoBtn = toolbar->addAction("Redo");
	redoBtn->setShortcut(QKeySequence::Redo);
	toolbar->addSeparator();

	QAction* moveBtn = toolbar->addAction("Move (M)");
	QAction* copyBtn = toolbar->addAction("Copy (C)");
	QAction* rotateBtn = toolbar->addAction("Rotate (R)");
	QAction* scaleBtn = toolbar->addAction("Scale");
	toolbar->addSeparator();

	QAction* osnapBtn = toolbar->addAction("OSNAP (F3)");
	osnapBtn->setCheckable(true);
	osnapBtn->setChecked(true);
	toolbar->addSeparator();

	QAction* quit = toolbar->addAction(QIcon(QPixmap("img/quit.png")), "Quit Application");

	connect(quit, &QAction::triggered, qApp, &QApplication::quit);
	connect(newa, &QAction::triggered, qApp, [this]()
		{
			mViewport->CreateNewScene();
		});
	connect(open, &QAction::triggered, qApp, [this]()
		{
			mViewport->LoadScene();
		});
	connect(save, &QAction::triggered, qApp, [this]()
		{
			mViewport->SaveScene();
		});
	connect(undoBtn, &QAction::triggered, [this]()
		{
			mViewport->Undo();
		});
	connect(redoBtn, &QAction::triggered, [this]()
		{
			mViewport->Redo();
		});
	connect(moveBtn, &QAction::triggered, [this]()
		{
			mViewport->UpdateState("TRANSFORM_MOVE");
		});
	connect(copyBtn, &QAction::triggered, [this]()
		{
			mViewport->UpdateState("TRANSFORM_COPY");
		});
	connect(rotateBtn, &QAction::triggered, [this]()
		{
			mViewport->UpdateState("TRANSFORM_ROTATE");
		});
	connect(scaleBtn, &QAction::triggered, [this]()
		{
			mViewport->UpdateState("TRANSFORM_SCALE");
		});
	connect(osnapBtn, &QAction::triggered, [this, osnapBtn]()
		{
			mViewport->ToggleOsnap();
			osnapBtn->setChecked(mViewport->IsOsnapEnabled());
		});
}

void MainWindow::SetSidebarWidget(QWidget* widget)
{
	mSidebar = new QWidget(widget);
	QVBoxLayout* vBox = new QVBoxLayout(mSidebar);
	QPushButton* btnLine = new QPushButton("Line", mSidebar);
	QPushButton* btnFace = new QPushButton("Face", mSidebar);

	vBox->addWidget(btnLine);
	vBox->addWidget(btnFace);
	mSidebar->setLayout(vBox);
	mSidebar->setFixedSize(90, 100);
	btnLine->setFixedHeight(35);
	btnFace->setFixedHeight(35);

	connect(btnLine, &QPushButton::clicked, [this]()
		{
			mViewport->UpdateState("DRAW_LINE");
		});
	connect(btnFace, &QPushButton::clicked, [this]()
		{
			mViewport->UpdateState("DRAW_FACE");
		});
}

void MainWindow::SetUnderbarWidget(QWidget* widget)
{
	mUnderbar = new QWidget(widget);
	QHBoxLayout* hBox = new QHBoxLayout(mUnderbar);
	QPushButton* btnPoint = new QPushButton("Point", mUnderbar);
	QPushButton* btnLine = new QPushButton("Line", mUnderbar);
	QPushButton* btnFace = new QPushButton("Face", mUnderbar);

	hBox->addWidget(btnPoint);
	hBox->addWidget(btnLine);
	hBox->addWidget(btnFace);
	mUnderbar->setLayout(hBox);
	mUnderbar->setFixedSize(300, 50);
	btnPoint->setFixedSize(90, 30);
	btnLine->setFixedSize(90, 30);
	btnFace->setFixedSize(90, 30);

	connect(btnPoint, &QPushButton::clicked, [this]()
		{
			mViewport->UpdateState("SELECT_POINT");
		});
	connect(btnLine, &QPushButton::clicked, [this]()
		{
			mViewport->UpdateState("SELECT_LINE");
		});
	connect(btnFace, &QPushButton::clicked, [this]()
		{
			mViewport->UpdateState("SELECT_FACE");
		});
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
	mSidebar->move(10, (height() - mSidebar->height()) / 3);
	mUnderbar->move((width() - mUnderbar->width()) / 2, (height() - mUnderbar->height()) - 100);
}
