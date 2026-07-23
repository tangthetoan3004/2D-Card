#include "TransformState.h"
#include "../viewport/Viewport.h"
#include "../command/MoveCommand.h"
#include "../command/CopyCommand.h"
#include "../command/RotateCommand.h"
#include "../command/ScaleCommand.h"
#include <cmath>

TransformState::TransformState(const std::string& name, SelectUtils::ViewportData* data, TransformMode mode)
    : State(name, data), mMode(mode), mHasBasePoint(false), mButton(Qt::NoButton)
{
    mName = name;
    mViewport = data->viewport;
    mScene = data->scene;
    mCamera = data->camera;
}

void TransformState::SetMode(TransformMode mode)
{
    mMode = mode;
    mHasBasePoint = false;
}

void TransformState::UpdateScene(Scene* scene)
{
    mScene = scene;
}

void TransformState::mousePressEvent(QMouseEvent* event)
{
    mButton = event->button();
}

void TransformState::mouseMoveEvent(QMouseEvent* event)
{
    mPos = event->pos();
    mViewport->update();
}

void TransformState::mouseReleaseEvent(QMouseEvent* event)
{
    if (mButton == Qt::LeftButton)
    {
        QPointF currentWorldPos = mCamera->SetWindowCoordinate(mPos.toPoint());
        if (mViewport->IsOsnapEnabled())
        {
            SelectUtils::SnapResult snap = SelectUtils::FindBestSnapPoint(mPos, mScene, mCamera);
            if (snap.type != SelectUtils::SnapType::NONE)
            {
                currentWorldPos = snap.snapWorldPoint;
            }
        }

        if (!mHasBasePoint)
        {
            mBasePoint = currentWorldPos;
            mHasBasePoint = true;
        }
        else
        {
            // Gather vertices to transform
            std::vector<Vertex*> targetVertices;
            std::list<Shape*> shapes = mScene->retShapes();
            for (Shape* s : shapes)
            {
                if (s && mScene->GetLayerManager())
                {
                    auto layer = mScene->GetLayerManager()->GetLayer(s->GetLayerName());
                    if (layer && layer->isLocked)
                    {
                        continue; // Skip shapes on locked layers
                    }
                }

                if (s->CheckShapeType("Point"))
                {
                    Vertex* v = dynamic_cast<Vertex*>(s);
                    if (v) targetVertices.push_back(v);
                }
                else if (s->CheckShapeType("Line"))
                {
                    Line* l = dynamic_cast<Line*>(s);
                    if (l)
                    {
                        auto ptrs = l->GetVertexPtrs();
                        for (auto* p : ptrs) if (p) targetVertices.push_back(p);
                    }
                }
                else if (s->CheckShapeType("Face"))
                {
                    Face* f = dynamic_cast<Face*>(s);
                    if (f)
                    {
                        auto ptrs = f->GetVertexPtrs();
                        for (auto* p : ptrs) if (p) targetVertices.push_back(p);
                    }
                }
            }

            if (mMode == TransformMode::MOVE)
            {
                float dx = currentWorldPos.x() - mBasePoint.x();
                float dy = currentWorldPos.y() - mBasePoint.y();
                mViewport->PushCommand(std::make_unique<MoveCommand>(targetVertices, dx, dy));
            }
            else if (mMode == TransformMode::COPY)
            {
                float dx = currentWorldPos.x() - mBasePoint.x();
                float dy = currentWorldPos.y() - mBasePoint.y();

                std::vector<Shape*> newShapes;
                std::vector<Vertex*> newVertices;

                // Copy all lines and faces
                for (Shape* s : shapes)
                {
                    if (s->CheckShapeType("Line"))
                    {
                        Line* l = dynamic_cast<Line*>(s);
                        if (l)
                        {
                            auto ptrs = l->GetVertexPtrs();
                            if (ptrs.size() >= 2 && ptrs[0] && ptrs[1])
                            {
                                Vertex* nv1 = new Vertex(ptrs[0]->GetVertex().x() + dx, ptrs[0]->GetVertex().y() + dy);
                                Vertex* nv2 = new Vertex(ptrs[1]->GetVertex().x() + dx, ptrs[1]->GetVertex().y() + dy);
                                Line* nLine = new Line(nv1, nv2);
                                newVertices.push_back(nv1);
                                newVertices.push_back(nv2);
                                newShapes.push_back(nLine);
                            }
                        }
                    }
                }

                if (!newShapes.empty())
                {
                    mViewport->PushCommand(std::make_unique<CopyCommand>(mScene, newShapes, newVertices));
                }
            }
            else if (mMode == TransformMode::ROTATE)
            {
                float dx = currentWorldPos.x() - mBasePoint.x();
                float dy = currentWorldPos.y() - mBasePoint.y();
                float angleRad = std::atan2(dy, dx);
                mViewport->PushCommand(std::make_unique<RotateCommand>(targetVertices, mBasePoint, angleRad));
            }
            else if (mMode == TransformMode::SCALE)
            {
                float dist = std::hypot(currentWorldPos.x() - mBasePoint.x(), currentWorldPos.y() - mBasePoint.y());
                float scaleFactor = (dist > 0.1f) ? dist : 1.0f;
                mViewport->PushCommand(std::make_unique<ScaleCommand>(targetVertices, mBasePoint, scaleFactor));
            }

            mHasBasePoint = false;
        }
    }

    mButton = Qt::NoButton;
    mViewport->update();
}

void TransformState::paintEvent(QPainter* painter)
{
    SelectUtils::SnapResult snap;
    if (mViewport->IsOsnapEnabled())
    {
        snap = SelectUtils::FindBestSnapPoint(mPos, mScene, mCamera);
    }

    QPoint targetScreenPos = (snap.type != SelectUtils::SnapType::NONE) ? mCamera->SetScreenCoordinate(snap.snapWorldPoint) : mPos.toPoint();

    if (mHasBasePoint)
    {
        QPoint baseScreen = mCamera->SetScreenCoordinate(mBasePoint);
        painter->setPen(QPen(Qt::darkCyan, 2, Qt::DashLine));
        painter->drawLine(baseScreen, targetScreenPos);

        painter->setPen(QPen(Qt::red, 8, Qt::SolidLine, Qt::RoundCap));
        painter->drawPoint(baseScreen);
    }

    if (snap.type != SelectUtils::SnapType::NONE)
    {
        SelectUtils::DrawSnapMarker(painter, mCamera, snap);
    }

    painter->setPen(QPen(Qt::black));
    std::string modeStr = "MOVE";
    if (mMode == TransformMode::COPY) modeStr = "COPY";
    else if (mMode == TransformMode::ROTATE) modeStr = "ROTATE";
    else if (mMode == TransformMode::SCALE) modeStr = "SCALE";

    QPointF displayWorldPos = (snap.type != SelectUtils::SnapType::NONE) ? snap.snapWorldPoint : mCamera->SetWindowCoordinate(mPos.toPoint());

    painter->drawText(
        mPos.x() + 15,
        mPos.y() - 15,
        QString("[%1] x: %2, y: %3")
        .arg(modeStr.c_str())
        .arg(displayWorldPos.x(), 0, 'f', 2)
        .arg(displayWorldPos.y(), 0, 'f', 2)
    );
}
