#include "SelectUtils.h"
#include "../viewport/Viewport.h"
#include "../viewport/Scene.h"
#include "../viewport/Camera.h"

QPolygonF SelectUtils::CreatePointBoundingBox(Camera* cam, Vertex* v, int d)
{
	QPolygonF poly = QPolygonF();
	QPointF p = cam->SetScreenCoordinate(v->GetVertex());
	QPointF dt[2] = { QPointF(d, d), QPointF(-d, d) };
	QPointF points[4] =
	{
		p + dt[0],
		p + dt[1],
		p - dt[0],
		p - dt[1]
	};

	// Counter-Clockwise
	for (int i = 0; i < 4; i++)
	{
		poly << points[i];
	}

	return poly;
}

QPolygonF SelectUtils::CreateLineBoundingBox(Camera* cam, Line* l, int d)
{
	QPolygonF poly = QPolygonF();
	QLineF line = l->GetLine(cam);
	qreal rad = line.angle() * acos(-1) / 180; // PI = acos(-1)
	qreal dx = d * sin(rad);
	qreal dy = d * cos(rad);
	QPointF dt = QPointF(dx, dy);
	QPointF points[4] =
	{
		line.p1() + dt,
		line.p2() + dt,
		line.p2() - dt,
		line.p1() - dt
	};

	// Counter-Clockwise
	for (int i = 0; i < 4; i++)
	{
		poly << points[i];
	}

	return poly;
}

QPolygonF SelectUtils::CreateLineSelectionBox(Camera* cam, Line* l)
{
	QPolygonF poly = QPolygonF();
	QLineF line = l->GetLine(cam);

	// No need to consider polygon direction
	poly << line.p1()
		<< QPointF(line.p1().x(), line.p2().y())
		<< line.p2()
		<< QPointF(line.p2().x(), line.p1().y());

	return poly;
}

QPolygonF SelectUtils::CreateFaceSelectionBox(Camera* cam, Face* f)
{
	QPolygonF poly = f->GetFace(cam);
	SelectUtils::PointReal min = { INFINITY, INFINITY };
	SelectUtils::PointReal max = { -INFINITY, -INFINITY };

	for (int i = 0; i < poly.size(); i++)
	{
		if (min.x >= poly[i].x())
		{
			min.x = poly[i].x();
		}

		if (min.y >= poly[i].y())
		{
			min.y = poly[i].y();
		}

		if (max.x <= poly[i].x())
		{
			max.x = poly[i].x();
		}

		if (max.y <= poly[i].y())
		{
			max.y = poly[i].y();
		}
	}

	// Counter-Clockwise
	poly = QPolygonF();
	poly << QPointF(max.x, max.y)
		<< QPointF(min.x, max.y)
		<< QPointF(min.x, min.y)
		<< QPointF(max.x, min.y);

	return poly;
}

bool SelectUtils::HitTesting(const QPointF& pos, QPolygonF poly)
{
	int count = 0;

	for (int i = 0; i < poly.size(); i++)
	{
		// Cross check line of each point with next point
		int j = (i + 1) % static_cast<int>(poly.size());
		SelectUtils::PointReal p1 = { poly[i].x(), poly[i].y() }; // prev
		SelectUtils::PointReal p2 = { poly[j].x(), poly[j].y() }; // next

		if (p1.y < pos.y() != p2.y < pos.y())
		{
			// Relation: (x - curr.x) : (v.y() - curr.y) = (next.x - curr.x) : (next.y - curr.y)
			qreal x = (p2.x - p1.x) / (p2.y - p1.y) * (pos.y() - p1.y) + p1.x;

			if (pos.x() < x)
			{
				count += 1;
			}
		}
	}

	// Odd: In | Even: Out
	return (count % 2 == 1 ? true : false);
}

bool SelectUtils::CalculateIntersection(const QLineF& l1, const QLineF& l2, QPointF& outIntersection)
{
	return (l1.intersects(l2, &outIntersection) == QLineF::BoundedIntersection);
}

bool SelectUtils::CalculatePerpendicularFoot(const QPointF& pt, const QLineF& line, QPointF& outFoot)
{
	QPointF A = line.p1();
	QPointF B = line.p2();
	QPointF u = B - A;
	qreal lenSq = u.x() * u.x() + u.y() * u.y();
	if (lenSq < 1e-9) return false;

	qreal t = ((pt.x() - A.x()) * u.x() + (pt.y() - A.y()) * u.y()) / lenSq;
	if (t >= 0.0 && t <= 1.0)
	{
		outFoot = A + t * u;
		return true;
	}
	return false;
}

SelectUtils::SnapResult SelectUtils::FindBestSnapPoint(const QPointF& mouseScreenPos, Scene* scene, Camera* camera, float thresholdPx)
{
	SnapResult bestResult;
	if (!scene || !camera) return bestResult;

	QPointF mouseWorldPos = camera->SetWindowCoordinate(mouseScreenPos.toPoint());
	std::list<Shape*> shapes = scene->retShapes();

	float minEndpointDist = thresholdPx;
	float minMidpointDist = thresholdPx;
	float minIntersectionDist = thresholdPx;
	float minPerpendicularDist = thresholdPx;

	std::vector<QLineF> allLines;

	// Collect lines and endpoints
	for (Shape* s : shapes)
	{
		if (s->CheckShapeType("Point"))
		{
			Vertex* v = dynamic_cast<Vertex*>(s);
			if (v)
			{
				QPoint screenP = camera->SetScreenCoordinate(v->GetVertex());
				float dist = QLineF(screenP, mouseScreenPos).length();
				if (dist < minEndpointDist)
				{
					minEndpointDist = dist;
					bestResult.type = SnapType::ENDPOINT;
					bestResult.snapWorldPoint = v->GetVertex();
					bestResult.label = "Endpoint";
				}
			}
		}
		else if (s->CheckShapeType("Line"))
		{
			Line* l = dynamic_cast<Line*>(s);
			if (l)
			{
				auto ptrs = l->GetVertexPtrs();
				if (ptrs.size() >= 2 && ptrs[0] && ptrs[1])
				{
					QPointF p1 = ptrs[0]->GetVertex();
					QPointF p2 = ptrs[1]->GetVertex();
					allLines.push_back(QLineF(p1, p2));

					// Check endpoints
					for (auto* v : ptrs)
					{
						QPoint screenP = camera->SetScreenCoordinate(v->GetVertex());
						float dist = QLineF(screenP, mouseScreenPos).length();
						if (dist < minEndpointDist)
						{
							minEndpointDist = dist;
							bestResult.type = SnapType::ENDPOINT;
							bestResult.snapWorldPoint = v->GetVertex();
							bestResult.label = "Endpoint";
						}
					}

					// Check midpoint
					QPointF mid = (p1 + p2) / 2.0;
					QPoint screenMid = camera->SetScreenCoordinate(mid);
					float distMid = QLineF(screenMid, mouseScreenPos).length();
					if (distMid < minMidpointDist)
					{
						minMidpointDist = distMid;
						if (bestResult.type != SnapType::ENDPOINT)
						{
							bestResult.type = SnapType::MIDPOINT;
							bestResult.snapWorldPoint = mid;
							bestResult.label = "Midpoint";
						}
					}
				}
			}
		}
		else if (s->CheckShapeType("Face"))
		{
			Face* f = dynamic_cast<Face*>(s);
			if (f)
			{
				auto ptrs = f->GetVertexPtrs();
				for (size_t i = 0; i < ptrs.size(); i++)
				{
					Vertex* v1 = ptrs[i];
					Vertex* v2 = ptrs[(i + 1) % ptrs.size()];
					if (v1 && v2)
					{
						QPointF p1 = v1->GetVertex();
						QPointF p2 = v2->GetVertex();
						allLines.push_back(QLineF(p1, p2));

						// Endpoint
						QPoint screenP = camera->SetScreenCoordinate(p1);
						float dist = QLineF(screenP, mouseScreenPos).length();
						if (dist < minEndpointDist)
						{
							minEndpointDist = dist;
							bestResult.type = SnapType::ENDPOINT;
							bestResult.snapWorldPoint = p1;
							bestResult.label = "Endpoint";
						}

						// Midpoint
						QPointF mid = (p1 + p2) / 2.0;
						QPoint screenMid = camera->SetScreenCoordinate(mid);
						float distMid = QLineF(screenMid, mouseScreenPos).length();
						if (distMid < minMidpointDist && bestResult.type != SnapType::ENDPOINT)
						{
							minMidpointDist = distMid;
							bestResult.type = SnapType::MIDPOINT;
							bestResult.snapWorldPoint = mid;
							bestResult.label = "Midpoint";
						}
					}
				}
			}
		}
	}

	// Check Intersections if no endpoint/midpoint found
	if (bestResult.type == SnapType::NONE)
	{
		for (size_t i = 0; i < allLines.size(); i++)
		{
			for (size_t j = i + 1; j < allLines.size(); j++)
			{
				QPointF interPt;
				if (CalculateIntersection(allLines[i], allLines[j], interPt))
				{
					QPoint screenInter = camera->SetScreenCoordinate(interPt);
					float distInter = QLineF(screenInter, mouseScreenPos).length();
					if (distInter < minIntersectionDist)
					{
						minIntersectionDist = distInter;
						bestResult.type = SnapType::INTERSECTION;
						bestResult.snapWorldPoint = interPt;
						bestResult.label = "Intersection";
					}
				}
			}
		}
	}

	// Check Perpendicular Foot if still no snap
	if (bestResult.type == SnapType::NONE)
	{
		for (const auto& line : allLines)
		{
			QPointF footPt;
			if (CalculatePerpendicularFoot(mouseWorldPos, line, footPt))
			{
				QPoint screenFoot = camera->SetScreenCoordinate(footPt);
				float distFoot = QLineF(screenFoot, mouseScreenPos).length();
				if (distFoot < minPerpendicularDist)
				{
					minPerpendicularDist = distFoot;
					bestResult.type = SnapType::PERPENDICULAR;
					bestResult.snapWorldPoint = footPt;
					bestResult.label = "Perpendicular";
				}
			}
		}
	}

	return bestResult;
}

void SelectUtils::DrawSnapMarker(QPainter* painter, Camera* camera, const SnapResult& snap)
{
	if (snap.type == SnapType::NONE || !painter || !camera) return;

	QPoint screenPt = camera->SetScreenCoordinate(snap.snapWorldPoint);
	int s = 7; // marker size

	painter->save();

	if (snap.type == SnapType::ENDPOINT)
	{
		// Green Square
		painter->setPen(QPen(Qt::green, 2));
		painter->drawRect(screenPt.x() - s, screenPt.y() - s, s * 2, s * 2);
	}
	else if (snap.type == SnapType::MIDPOINT)
	{
		// Cyan Triangle
		painter->setPen(QPen(Qt::cyan, 2));
		QPolygon poly;
		poly << QPoint(screenPt.x(), screenPt.y() - s)
			<< QPoint(screenPt.x() - s, screenPt.y() + s)
			<< QPoint(screenPt.x() + s, screenPt.y() + s);
		painter->drawPolygon(poly);
	}
	else if (snap.type == SnapType::INTERSECTION)
	{
		// Orange Cross
		painter->setPen(QPen(QColor(255, 140, 0), 2));
		painter->drawLine(screenPt.x() - s, screenPt.y() - s, screenPt.x() + s, screenPt.y() + s);
		painter->drawLine(screenPt.x() - s, screenPt.y() + s, screenPt.x() + s, screenPt.y() - s);
	}
	else if (snap.type == SnapType::PERPENDICULAR)
	{
		// Purple Perpendicular symbol
		painter->setPen(QPen(QColor(148, 0, 211), 2));
		painter->drawLine(screenPt.x() - s, screenPt.y() + s, screenPt.x() + s, screenPt.y() + s);
		painter->drawLine(screenPt.x(), screenPt.y() - s, screenPt.x(), screenPt.y() + s);
		painter->drawLine(screenPt.x(), screenPt.y(), screenPt.x() + s / 2, screenPt.y());
	}

	// Tooltip label next to snap marker
	painter->setPen(QPen(Qt::darkGreen));
	painter->setFont(QFont("Arial", 9, QFont::Bold));
	painter->drawText(screenPt.x() + 12, screenPt.y() + 4, snap.label);

	painter->restore();
}
