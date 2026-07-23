#include "State.h"
#include "../viewport/Viewport.h"
#include "../command/DrawLineCommand.h"

DrawLineState::DrawLineState(const std::string& name, SelectUtils::ViewportData* data)
	: State(name, data)
{
	mName = name;
	mViewport = data->viewport;
	mScene = data->scene;
	mCamera = data->camera;
	mPoints = {};
	mDrawLine = false;
}

void DrawLineState::UpdateScene(Scene* scene)
{
	mScene = scene;
}

void DrawLineState::mousePressEvent(QMouseEvent* event)
{
	mButton = event->button();
}

void DrawLineState::mouseMoveEvent(QMouseEvent* event)
{
	mPos = event->pos();
	mViewport->update();
}

void DrawLineState::mouseReleaseEvent(QMouseEvent* event)
{
	if (mButton == Qt::LeftButton)
	{
		QPointF currentPos = mPos;
		if (mViewport->IsOsnapEnabled())
		{
			SelectUtils::SnapResult snap = SelectUtils::FindBestSnapPoint(mPos, mScene, mCamera);
			if (snap.type != SelectUtils::SnapType::NONE)
			{
				currentPos = mCamera->SetScreenCoordinate(snap.snapWorldPoint);
			}
		}

		mPoints.push_back(currentPos);
		mDrawLine = true;

		if (mPoints.size() == 2)
		{
			std::string activeLayer = mScene->GetLayerManager() ? mScene->GetLayerManager()->GetActiveLayerName() : "0";
			Vertex* v1 = new Vertex(mCamera->SetWindowCoordinate(mPoints.front().toPoint()));
			Vertex* v2 = new Vertex(mCamera->SetWindowCoordinate(mPoints.back().toPoint()));
			Line* line = new Line(v1, v2);
			v1->SetLayerName(activeLayer);
			v2->SetLayerName(activeLayer);
			line->SetLayerName(activeLayer);

			mViewport->PushCommand(std::make_unique<DrawLineCommand>(mScene, line, std::vector<Vertex*>{ v1, v2 }));

			// Initialization
			mPoints = {};
			mDrawLine = false;
		}
	}

	mButton = Qt::NoButton;
	mViewport->update();
}

void DrawLineState::paintEvent(QPainter* painter)
{
	SelectUtils::SnapResult snap;
	if (mViewport->IsOsnapEnabled())
	{
		snap = SelectUtils::FindBestSnapPoint(mPos, mScene, mCamera);
	}

	if (mDrawLine && mPoints.size() != 2)
	{
		QPointF targetPos = (snap.type != SelectUtils::SnapType::NONE) ? mCamera->SetScreenCoordinate(snap.snapWorldPoint) : mPos;

		// Draw line first
		painter->setPen(QPen(Qt::darkGray, 3));
		painter->drawLine(mPoints.at(0), targetPos);

		// Draw points on line
		painter->setPen(QPen(Qt::blue, 10, Qt::SolidLine, Qt::RoundCap));
		painter->drawPoint(mPoints.at(0));
		painter->drawPoint(targetPos);
	}

	if (snap.type != SelectUtils::SnapType::NONE)
	{
		SelectUtils::DrawSnapMarker(painter, mCamera, snap);
	}

	painter->setPen(QPen(Qt::black));
	QPointF labelWorldPos = (snap.type != SelectUtils::SnapType::NONE) ? snap.snapWorldPoint : mCamera->SetWindowCoordinate(mPos.toPoint());
	painter->drawText(
		mPos.x() + 15,
		mPos.y() - 15,
		QString("x: %1, y: %2")
		.arg(labelWorldPos.x(), 0, 'f', 2)
		.arg(labelWorldPos.y(), 0, 'f', 2)
	);
}
