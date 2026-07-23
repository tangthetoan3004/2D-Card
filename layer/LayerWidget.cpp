#include "LayerWidget.h"
#include "../viewport/Viewport.h"

LayerWidget::LayerWidget(LayerManager* manager, Viewport* viewport, QWidget* parent)
	: QWidget(parent), mManager(manager), mViewport(viewport)
{
	SetupUI();
	RefreshTable();
}

void LayerWidget::SetupUI()
{
	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	mainLayout->setContentsMargins(5, 5, 5, 5);

	// Action buttons
	QHBoxLayout* btnLayout = new QHBoxLayout();
	mAddBtn = new QPushButton("+ New Layer", this);
	mDeleteBtn = new QPushButton("- Delete", this);
	btnLayout->addWidget(mAddBtn);
	btnLayout->addWidget(mDeleteBtn);
	mainLayout->addLayout(btnLayout);

	// Table Widget
	mTable = new QTableWidget(this);
	mTable->setColumnCount(5);
	QStringList headers;
	headers << "Active" << "Name" << "Color" << "Vis" << "Lock";
	mTable->setHorizontalHeaderLabels(headers);
	mTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	mTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
	mTable->setSelectionBehavior(QAbstractItemView::SelectRows);
	mTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

	mainLayout->addWidget(mTable);

	connect(mAddBtn, &QPushButton::clicked, this, &LayerWidget::OnAddLayer);
	connect(mDeleteBtn, &QPushButton::clicked, this, &LayerWidget::OnDeleteLayer);
	connect(mTable, &QTableWidget::cellClicked, this, &LayerWidget::OnCellClicked);
}

void LayerWidget::RefreshTable()
{
	if (!mManager) return;

	auto layers = mManager->GetAllLayers();
	mTable->setRowCount(static_cast<int>(layers.size()));

	std::string activeName = mManager->GetActiveLayerName();

	for (size_t i = 0; i < layers.size(); i++)
	{
		auto layer = layers[i];
		int row = static_cast<int>(i);

		// Column 0: Active
		QTableWidgetItem* activeItem = new QTableWidgetItem(layer->name == activeName ? "[*]" : "");
		activeItem->setTextAlignment(Qt::AlignCenter);
		mTable->setItem(row, 0, activeItem);

		// Column 1: Name
		QTableWidgetItem* nameItem = new QTableWidgetItem(QString::fromStdString(layer->name));
		mTable->setItem(row, 1, nameItem);

		// Column 2: Color
		QTableWidgetItem* colorItem = new QTableWidgetItem();
		colorItem->setBackground(layer->color);
		mTable->setItem(row, 2, colorItem);

		// Column 3: Vis
		QTableWidgetItem* visItem = new QTableWidgetItem(layer->isVisible ? "Show" : "Hide");
		visItem->setTextAlignment(Qt::AlignCenter);
		if (!layer->isVisible) visItem->setForeground(Qt::gray);
		mTable->setItem(row, 3, visItem);

		// Column 4: Lock
		QTableWidgetItem* lockItem = new QTableWidgetItem(layer->isLocked ? "Lock" : "Free");
		lockItem->setTextAlignment(Qt::AlignCenter);
		if (layer->isLocked) lockItem->setForeground(Qt::red);
		mTable->setItem(row, 4, lockItem);
	}
}

void LayerWidget::OnAddLayer()
{
	bool ok = false;
	QString name = QInputDialog::getText(this, "New Layer", "Enter Layer Name:", QLineEdit::Normal, "", &ok);
	if (ok && !name.trimmed().isEmpty())
	{
		QColor color = QColorDialog::getColor(Qt::black, this, "Select Layer Color");
		if (color.isValid())
		{
			if (mManager->AddLayer(name.trimmed().toStdString(), color))
			{
				RefreshTable();
				if (mViewport) mViewport->update();
			}
			else
			{
				QMessageBox::warning(this, "Error", "Layer name already exists or is invalid.");
			}
		}
	}
}

void LayerWidget::OnDeleteLayer()
{
	int row = mTable->currentRow();
	if (row < 0) return;

	QString name = mTable->item(row, 1)->text();
	if (name == "0")
	{
		QMessageBox::warning(this, "Warning", "Layer '0' cannot be deleted.");
		return;
	}

	if (mManager->RemoveLayer(name.toStdString()))
	{
		RefreshTable();
		if (mViewport) mViewport->update();
	}
}

void LayerWidget::OnCellClicked(int row, int column)
{
	if (row < 0 || !mManager) return;
	QString name = mTable->item(row, 1)->text();
	std::string stdName = name.toStdString();
	auto layer = mManager->GetLayer(stdName);
	if (!layer) return;

	if (column == 0) // Set Active
	{
		mManager->SetActiveLayer(stdName);
		RefreshTable();
	}
	else if (column == 2) // Change Color
	{
		QColor newColor = QColorDialog::getColor(layer->color, this, "Select Layer Color");
		if (newColor.isValid())
		{
			mManager->SetLayerColor(stdName, newColor);
			RefreshTable();
			if (mViewport) mViewport->update();
		}
	}
	else if (column == 3) // Toggle Visible
	{
		mManager->SetLayerVisible(stdName, !layer->isVisible);
		RefreshTable();
		if (mViewport) mViewport->update();
	}
	else if (column == 4) // Toggle Lock
	{
		mManager->SetLayerLocked(stdName, !layer->isLocked);
		RefreshTable();
		if (mViewport) mViewport->update();
	}
}
