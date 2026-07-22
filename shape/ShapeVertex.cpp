#include <cmath>
#include "Shape.h"

Vertex::Vertex(float x, float y)
{
	mType = "Point";
	mX = x;
	mY = y;
}

Vertex::Vertex(const QPointF& p)
{
	mType = "Point";
	mX = p.x();
	mY = p.y();
}

QPointF Vertex::GetVertex()
{
	return QPointF(mX, mY);
}

void Vertex::UpdateVertex(const QPointF& p)
{
	mX = p.x();
	mY = p.y();
}

void Vertex::Move(float dx, float dy)
{
	mX += dx;
	mY += dy;
}

void Vertex::Rotate(const QPointF& pivot, float angleRad)
{
	float dx = mX - pivot.x();
	float dy = mY - pivot.y();
	mX = pivot.x() + (dx * std::cos(angleRad) - dy * std::sin(angleRad));
	mY = pivot.y() + (dx * std::sin(angleRad) + dy * std::cos(angleRad));
}

void Vertex::Scale(const QPointF& pivot, float scaleFactor)
{
	mX = pivot.x() + scaleFactor * (mX - pivot.x());
	mY = pivot.y() + scaleFactor * (mY - pivot.y());
}

std::string Vertex::Type()
{
	return mType;
}

bool Vertex::CheckShapeType(const std::string& s)
{
	return !mType.compare(s);
}

void Vertex::Render(QPainter* painter, Camera* camera)
{
	QPoint p = camera->SetScreenCoordinate(QPointF(mX, mY));
	painter->setPen(QPen(Qt::blue, 10, Qt::SolidLine, Qt::RoundCap));
	painter->drawPoint(p);
}
