#pragma once
#include <string>
#include "BSplineCurve.h"
#include "BSplineSurface.h"

class CSVExporter
{
public:

	// Curve
	static bool ExportControlPoints(const BSplineCurve& curve, const std::string& fileName);
	static bool ExportKnotVector(const BSplineCurve& curve, const std::string& fileName);
	static bool ExportCurveSamples(const BSplineCurve& curve, const std::string& fileName, int sampleCount);
	static bool ExportBasisFunctions(const BSplineCurve& curve, const std::string& fileName, int sampleCount);

	//Surface
	static bool ExportSurfaceSamples(const BSplineSurface& surface, const std::string& fileName, int sampleCount);
};