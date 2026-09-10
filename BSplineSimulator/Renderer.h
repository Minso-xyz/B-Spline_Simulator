#pragma once
#include "Point3D.h"
#include "Line3D.h"
#include "Vertex.h"
#include "Edge.h"
#include "Circle3D.h"
#include "BSplineCurve.h"

class Renderer
{
public:

	void DrawCoordinateAxis();
	void DrawPoint(const Point3D& point);
	void DrawLine(const Line3D& line);
	void DrawCircle(const Circle3D& circle);
	void DrawPolyline(const std::vector<Point3D>& points);
	void DrawControlPolygon(std::vector < Point3D> controlPoints);
	void DrawBSplineCurve(const BSplineCurve& curve);
	void DrawVertex(const Vertex& vertex);
	void DrawEdge(const Edge& edge);
	void DrawCube();
};