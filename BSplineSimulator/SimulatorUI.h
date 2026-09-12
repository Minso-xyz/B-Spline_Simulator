#pragma once
#include "BSplineCurve.h"
#include "Camera.h"
#include "SimulatorUI.h"
#include "CSVExporter.h"

class SimulatorUI
{
private:
	int EditingDegree;
	int EditingControlPoint;
	std::vector<Point3D> EditingControlPoints;
	int EditingControlPointCount;
	int SampleCount;

public:
	SimulatorUI(const BSplineCurve& curve);
	bool Draw(BSplineCurve& curve, Camera& camera, CSVExporter& dataCSV);

private:
	void ResizeControlPoints();
	std::vector<double> CreateClampedUniformKnots(int controlPointCount, int degree) const;
};