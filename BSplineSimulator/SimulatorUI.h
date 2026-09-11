#pragma once
#include "BSplineCurve.h"

class SimulatorUI
{
private:
	int EditingDegree;
	int EditingControlPoint;
	std::vector<Point3D> EditingControlPoints;
	int EditingControlPointCount;

//public:
//	int Degree = 3;

public:
	SimulatorUI(const BSplineCurve& curve);
	bool Draw(BSplineCurve& curve);

private:
	void ResizeControlPoints();
	std::vector<double> CreateClampedUniformKnots(int controlPointCount, int degree) const;
};