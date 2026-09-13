#pragma once
#include "BSplineCurve.h"
#include "Camera.h"
#include "SimulatorUI.h"
#include "CSVExporter.h"
#include "BSplineSurface.h"

class SimulatorUI
{
public:
	int EditingDegree;
	int EditingControlPoint;
	std::vector<Point3D> EditingControlPoints;
	int EditingControlPointCount;
	int SampleCount;
	int SampleCountSurface;

	bool ShowCurve;
	bool ShowSurface;
	bool ShowControlNet;

public:
	SimulatorUI(const BSplineCurve& curve);
	bool Draw(BSplineCurve& curve, BSplineSurface& surface, Camera& camera, CSVExporter& dataCSV);

private:
	void ResizeControlPoints();
	std::vector<double> CreateClampedUniformKnots(int controlPointCount, int degree) const;
};