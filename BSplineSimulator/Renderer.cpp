#include "Renderer.h"
#include <GLFW/glfw3.h>
#include <vector>
#include "BSplineCurve.h"
#include "BSplineSurface.h"

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

void Renderer::DrawControlPolygon(std::vector<Point3D> controlPoints)
{
	// Draw points
	for (int i = 0; i< controlPoints.size(); i++)
	{
		DrawPoint(controlPoints[i]);
	}

	// Draw lines
	for (int i = 0; i < controlPoints.size() - 1; i++)
	{
		glLineWidth(2.0f);
		glBegin(GL_LINES);
		glColor3f(1.0f, 0.0f, 1.0f);
		glVertex3f((float)controlPoints[i].X, (float)controlPoints[i].Y, (float)controlPoints[i].Z);
		glVertex3f((float)controlPoints[i + 1].X, (float)controlPoints[i + 1].Y, (float)controlPoints[i + 1].Z);
		glEnd();
	}
}

void Renderer::DrawBSplineCurve(const BSplineCurve& curve)
{
	std::vector<Point3D> points = curve.CreatePolyline(100);
	DrawPolyline(points);
}

void Renderer::DrawBSplineSurface(const BSplineSurface& surface, int sampleCount)
{
	for (int i = 0; i <= sampleCount; i++)
	{
		double u = static_cast<double>(i) / static_cast<double>(sampleCount);
		std::vector<Point3D> pointsU = surface.CreateIsoCurveU(u, sampleCount);
		DrawPolyline(pointsU);
	}

	for (int j = 0; j <= sampleCount; j++)
	{
		double v = static_cast<double>(j) / static_cast<double>(sampleCount);
		std::vector<Point3D> pointsV = surface.CreateIsoCurveV(v, sampleCount);
		DrawPolyline(pointsV);
	}
}

void Renderer::DrawVertex(const Vertex& vertex)
{
	Renderer::DrawPoint(vertex.Position);
}

void Renderer::DrawEdge(const Edge& edge)
{
	DrawLine(Line3D(edge.Start.Position, edge.End.Position));
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
