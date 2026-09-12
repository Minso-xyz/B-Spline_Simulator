#include "CSVExporter.h"
#include <fstream>
#include <iomanip>

bool CSVExporter::ExportControlPoints(const BSplineCurve& curve, const std::string& fileName)
{
	// create a new csv file or overwrite if the file already exists
	std::ofstream file(fileName);

	// returns false in case the file couldnt be generated (the file is open)
	if (!file.is_open())
	{
		return false;
	}

	// set the precision of the numbers exported
	file << std::fixed << std::setprecision(6);

	// CSV header
	file << "Point,X,Y,Z\n";

	// write each control point at each line
	for (std::size_t i = 0; i < curve.ControlPoints.size(); ++i)
	{
		const Point3D& point = curve.ControlPoints[i];

		file <<"P" << i << ", "
			<< point.X << ","
			<< point.Y << ","
			<< point.Z << "\n";
	}

	file.close();

	return true;
}

bool CSVExporter::ExportKnotVector(const BSplineCurve& curve, const std::string& fileName)
{
	// create a new csv file or overwrite if the file already exists
	std::ofstream file(fileName);

	// returns false in case the file couldnt be generated (the file is open)
	if (!file.is_open())
	{
		return false;
	}

	// set the precision of the numbers exported
	file << std::fixed << std::setprecision(6);

	// CSV header
	file << "Index,Knot\n";

	// write each knot at each line
	for (std::size_t i = 0; i < curve.Knots.size(); ++i)
	{
		const double& knot = curve.Knots[i];

		file << i << ", "
			<< knot << "\n";
	}

	file.close();

	return true;
}

bool CSVExporter::ExportCurveSamples(const BSplineCurve& curve, const std::string& fileName, int sampleCount)
{
	// create a new csv file or overwrite if the file already exists
	std::ofstream file(fileName);

	// returns false in case the file couldnt be generated (the file is open)
	if (!file.is_open())
	{
		return false;
	}

	// set the precision of the numbers exported
	file << std::fixed << std::setprecision(6);

	// CSV header
	file << "t,X,Y,Z\n";

	// write each Evaluate value at each line
	for (std::size_t i = 0; i < sampleCount; ++i)
	{
		double t = (double)i / sampleCount;
		const Point3D point = curve.Evaluate(t);

		file << t << ", "
			<< point.X << ","
			<< point.Y << ","
			<< point.Z << "\n";
	}

	file.close();

	return true;
 }