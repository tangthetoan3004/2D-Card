#pragma once
#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QColorDialog>
#include <QInputDialog>
#include <QMessageBox>
#include "Layer.h"

class Viewport;

class LayerWidget : public QWidget
{
	Q_OBJECT

public:
	LayerWidget(LayerManager* manager, Viewport* viewport, QWidget* parent = nullptr);
	~LayerWidget() = default;

	void RefreshTable();

private slots:
	void OnAddLayer();
	void OnDeleteLayer();
	void OnCellClicked(int row, int column);

private:
	LayerManager* mManager;
	Viewport* mViewport;
	QTableWidget* mTable;
	QPushButton* mAddBtn;
	QPushButton* mDeleteBtn;

	void SetupUI();
};
