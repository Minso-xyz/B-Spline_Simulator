#include "Renderer.h"
#include <GLFW/glfw3.h>
#include <vector>
#include "BSplineCurve.h"

void Renderer::DrawPoint(const Point3D& point)
{
	glPointSize(10.0f);
	glBegin(GL_POINTS);
	glColor3f(1.0f, 0.0f, 1.0f);
	glVertex3f((float)point.X, (float)point.Y, (float)point.Z);
	glEnd();
}

void Renderer::DrawLine(const Line3D& line)
{
	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f((float)line.Start.X, (float)line.Start.Y, (float)line.Start.Z);
	glVertex3f((float)line.End.X, (float)line.End.Y, (float)line.End.Z);
	glEnd();
}

void Renderer::DrawCircle(const Circle3D& circle)
{
	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3f(1.0f, 1.0f, 1.0f);

	int segmentCount = 64;
	std::vector<Point3D> points = circle.CreatePolyline(segmentCount);

	for (int i = 0; i < points.size() - 1; i++)
	{
		DrawLine(Line3D(points[i],points[i + 1]));
	}
}

void Renderer::DrawPolyline(const std::vector<Point3D>& points)
{
	glLineWidth(2.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_LINE_STRIP);

	for (const Point3D& point : points)
	{
		glVertex3d(
			point.X,
			point.Y,
			point.Z
		);
	}
	glEnd();
}

void Renderer::DrawBSplineCurve(const BSplineCurve& curve)
{
	std::vector<Point3D> points = curve.CreatePolyline(100);
	DrawPolyline(points);
}

void Renderer::DrawVertex(const Vertex& vertex)
{
	Renderer::DrawPoint(vertex.Position);
}

void Renderer::DrawEdge(const Edge& edge)
{
	DrawLine(Line3D(edge.Start.Position, edge.End.Position));
}

void Renderer::DrawCube()
{
	// Draw Cube
	// 8 vertexes
	Vertex V0(Point3D(-0.5, -0.5, -0.5));
	Vertex V1(Point3D(0.5, -0.5, -0.5));
	Vertex V2(Point3D(0.5, 0.5, -0.5));
	Vertex V3(Point3D(-0.5, 0.5, -0.5));
	Vertex V4(Point3D(-0.5, -0.5, 0.5));
	Vertex V5(Point3D(0.5, -0.5, 0.5));
	Vertex V6(Point3D(0.5, 0.5, 0.5));
	Vertex V7(Point3D(-0.5, 0.5, 0.5));

	// 12 edges
	Edge E0(V0, V1);
	Edge E1(V1, V2);
	Edge E2(V2, V3);
	Edge E3(V3, V0);
	Edge E4(V4, V5);
	Edge E5(V5, V6);
	Edge E6(V6, V7);
	Edge E7(V7, V4);
	Edge E8(V0, V4);
	Edge E9(V1, V5);
	Edge E10(V2, V6);
	Edge E11(V3, V7);

	std::vector<Edge> edges =
	{
		E0, E1, E2, E3,
		E4, E5, E6, E7,
		E8, E9, E10, E11
	};

	for (const Edge& edge : edges)
	{
		Renderer::DrawEdge(edge);
	}
}

void Renderer::DrawCoordinateAxis()
{
	glLineWidth(1.0f);
	glBegin(GL_LINES);

	// X-Axis (Red)
	glColor3f(150.0f, 0.0f, 0.0f);
	glVertex3f(-150.0f, 0.0f, 0.0f);
	glVertex3f(150.0f, 0.0f, 0.0f);

	// Y-Axis (Green)
	glColor3f(0.0f, 200.0f, 0.0f);
	glVertex3f(0.0f, -200.0f, 0.0f);
	glVertex3f(0.0f, 200.0f, 0.0f);

	// Z-Axis (Blue)
	glColor3f(0.0f, 0.0f, 150.0f);
	glVertex3f(0.0f, 0.0f, -150.0f);
	glVertex3f(0.0f, 0.0f, 150.0f);

	glEnd();
}
