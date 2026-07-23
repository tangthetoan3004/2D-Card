#include "State.h"
#include "../viewport/Viewport.h"
#include "../command/DrawFaceCommand.h"

DrawFaceState::DrawFaceState(const std::string& name, SelectUtils::ViewportData* data)
	: State(name, data)
{
	mName = name;
	mViewport = data->viewport;
	mScene = data->scene;
	mCamera = data->camera;
	mPoints = {};
	mDrawPolygon = false;
	mVertex = new Vertex(QPoint(INFINITY, INFINITY));
}

void DrawFaceState::UpdateScene(Scene* scene)
{
	mScene = scene;
}

void DrawFaceState::mousePressEvent(QMouseEvent* event)
{
	mButton = event->button();
}

void DrawFaceState::mouseMoveEvent(QMouseEvent* event)
{
	mPos = event->pos();

	if (mPoints.size() && ((mButton != Qt::LeftButton) || (mButton == Qt::LeftButton)))
	{
		// Create bounding box and hit testing by start point.
		mLine = QLineF(mPoints.back(), mPos);
		mVertex = new Vertex(mCamera->SetWindowCoordinate(mPoints[0].toPoint()));
		mPolygon = SelectUtils::CreatePointBoundingBox(mCamera, mVertex, 15); // Start point's bounding box
		mHit = SelectUtils::HitTesting(mPos, mPolygon);

		if (mHit && (mPoints.size() > 1))
		{
			mGuidePolygon = QPolygonF();

			for (int i = 0; i < mPoints.size(); i++)
			{
				mGuidePolygon << mPoints[i];
			}

			mGuidePolygon << mPos;
		}
	}

	mViewport->update();
}

void DrawFaceState::mouseReleaseEvent(QMouseEvent* event)
{
	if (mButton == Qt::LeftButton)
	{
		QPointF currentPos = mPos;
		if (mViewport->IsOsnapEnabled() && !mHit)
		{
			SelectUtils::SnapResult snap = SelectUtils::FindBestSnapPoint(mPos, mScene, mCamera);
			if (snap.type != SelectUtils::SnapType::NONE)
			{
				currentPos = mCamera->SetScreenCoordinate(snap.snapWorldPoint);
			}
		}

		mPoints.push_back(currentPos);
		mDrawPolygon = true;

		// Draw finish and Initialization
		if (mHit && (mPoints.size() > 2))
		{
			if (mPoints.size() != 3)
			{
				std::string activeLayer = mScene->GetLayerManager() ? mScene->GetLayerManager()->GetActiveLayerName() : "0";

				// Add vertices to Scene and polygon without last vertex
				for (int i = 0; i < mPoints.size() - 1; i++)
				{
					Vertex* v = new Vertex(mCamera->SetWindowCoordinate(mPoints[i].toPoint()));
					v->SetLayerName(activeLayer);
					vertices.push_back(v);
					vecVertices.push_back(v);
				}

				Face* face = new Face(vertices);
				face->SetLayerName(activeLayer);
				mViewport->PushCommand(std::make_unique<DrawFaceCommand>(mScene, face, vecVertices));
			}

			mPoints = {};
			mDrawPolygon = false;
			mGuidePolygon = QPolygonF();
		}
	}

	mButton = Qt::NoButton;
	mViewport->update();
}

void DrawFaceState::paintEvent(QPainter* painter)
{
	if (mDrawPolygon)
	{
		painter->setPen(QPen(Qt::darkGray, 3));
		painter->drawLine(mLine);

		for (int i = 0; i < mPoints.size(); i++)
		{
			// Draw polygon lines
			int j = (i + 1) % static_cast<int>(mPoints.size());

			if (j > 0)
			{
				mLine = QLineF(mPoints[i], mPoints[j]);
				painter->setPen(QPen(Qt::darkGray, 3));
				painter->drawLine(mLine);
			}

			// Draw polygon points
			painter->setPen(QPen(Qt::blue, 10, Qt::SolidLine, Qt::RoundCap));
			painter->drawPoint(mPoints[i]);
			painter->drawPoint(mPos);
		}

		if (mHit && (mPoints.size() > 1))
		{
			// Draw guide polygon
			painter->setPen(QPen(Qt::darkGray, 3));
			painter->setBrush(QBrush(Qt::lightGray));
			painter->drawPolygon(mGuidePolygon, Qt::OddEvenFill);

			// Draw polygon points again for priority
			for (int i = 0; i < mPoints.size(); i++)
			{
				painter->setPen(QPen(Qt::blue, 10, Qt::SolidLine, Qt::RoundCap));
				painter->drawPoint(mPoints[i]);
				painter->drawPoint(mPos);
			}

			// Draw mouse hover points
			painter->setPen(QPen(Qt::red, 10, Qt::SolidLine, Qt::RoundCap));
			painter->setBrush(QBrush());
			painter->drawPoint(mPoints[0]);
			painter->drawPoint(mPos);

			// Draw bounding box with start point
			painter->setPen(QPen(Qt::blue, 2));
			painter->drawPolygon(mPolygon);
		}
	}

	SelectUtils::SnapResult snap;
	if (mViewport->IsOsnapEnabled())
	{
		snap = SelectUtils::FindBestSnapPoint(mPos, mScene, mCamera);
		if (snap.type != SelectUtils::SnapType::NONE)
		{
			SelectUtils::DrawSnapMarker(painter, mCamera, snap);
		}
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
