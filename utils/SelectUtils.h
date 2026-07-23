#pragma once
#include <QPoint>
#include <QPolygon>
#include "../shape/Shape.h"

class Viewport;
class Scene;
class Camera;

namespace SelectUtils
{
	struct ViewportData
	{
		Viewport* viewport;
		Scene* scene;
		Camera* camera;
	};

	struct Point
	{
		int x;
		int y;
	};

	struct PointReal
	{
		qreal x;
		qreal y;
	};

	enum class SnapType
	{
		NONE,
		ENDPOINT,
		MIDPOINT,
		INTERSECTION,
		PERPENDICULAR
	};

	struct SnapResult
	{
		SnapType type = SnapType::NONE;
		QPointF snapWorldPoint;
		QString label;
	};

	QPolygonF CreatePointBoundingBox(Camera* cam, Vertex* v, int d);
	QPolygonF CreateLineBoundingBox(Camera* cam, Line* l, int d);
	QPolygonF CreateLineSelectionBox(Camera* cam, Line* l);
	QPolygonF CreateFaceSelectionBox(Camera* cam, Face* f);
	bool HitTesting(const QPointF& pos, QPolygonF poly);

	// OSNAP Functions
	bool CalculateIntersection(const QLineF& l1, const QLineF& l2, QPointF& outIntersection);
	bool CalculatePerpendicularFoot(const QPointF& pt, const QLineF& line, QPointF& outFoot);
	SnapResult FindBestSnapPoint(const QPointF& mouseScreenPos, Scene* scene, Camera* camera, float thresholdPx = 12.0f);
	void DrawSnapMarker(QPainter* painter, Camera* camera, const SnapResult& snap);
}
