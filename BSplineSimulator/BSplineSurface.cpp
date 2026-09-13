#include "BSplineSurface.h"

Point3D BSplineSurface::Evaluate(double u, double v) const
{
	Point3D result(0, 0, 0);

	for (int i = 0; i < ControlNet.size(); i++)  // number of rows [u]
	{
		for (int j = 0; j < ControlNet[i].size(); j++)  // number of columns [v]
		{
			double Nu = BasisFunction(i, DegreeU, u, KnotsU);

			double Nv = BasisFunction(j, DegreeV, v, KnotsV);

			result = result + ControlNet[i][j] * (Nu * Nv);
		}
	}
	return result;
}

double BSplineSurface::BasisFunction(int i, int degree, double t, const std::vector<double>& knots) const
{
	if (degree == 0)
	{
		if ((knots[i] <= t && t < knots[i + 1]) || (t == knots.back() && i == ControlNet.size() - 1))
		{
			return 1.0;
		}
		return 0.0;
	}

	double left = 0;
	double right = 0;

	double denom1 = knots[i + degree] - knots[i];

	if (denom1 != 0)
	{
		left = ((t - knots[i]) / denom1) * BasisFunction(i, degree - 1, t, knots);
	}

	double denom2 = knots[i + degree + 1] - knots[i + 1];

	if (denom2 != 0)
	{
		right = (knots[i + degree + 1] - t) / denom2 * BasisFunction(i + 1, degree - 1, t, knots);
	}
	return left + right;
}

std::vector<Point3D> BSplineSurface::CreateIsoCurveU(double u, int sampleCount)
{
	std::vector<Point3D> points;

	for (int i = 0; i < sampleCount; i++)
	{
		double v = static_cast<double>(i) / static_cast<double>(sampleCount);
		points.push_back(Evaluate(u, v));
	}
	return points;
}

std::vector<Point3D> BSplineSurface::CreateIsoCurveV(double v, int sampleCount)
{
	std::vector<Point3D> points;

	for (int i = 0; i < sampleCount; i++)
	{
		double u = static_cast<double>(i) / static_cast<double>(sampleCount);
		points.push_back(Evaluate(u, v));
	}
	return points;
}

