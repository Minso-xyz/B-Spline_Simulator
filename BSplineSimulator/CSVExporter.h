#pragma once
#include <string>
#include "BSplineCurve.h"

class CSVExporter
{
public:
	static bool ExportControlPoints(const BSplineCurve& curve, const std::string& fileName);
	static bool ExportKnotVector(const BSplineCurve& curve, const std::string& fileName);
	static bool ExportCurveSamples(const BSplineCurve& curve, const std::string& fileName, int sampleCount);
	static bool ExportBasisFunctions(const BSplineCurve& curve, const std::string& fileName, int sampleCount);
};