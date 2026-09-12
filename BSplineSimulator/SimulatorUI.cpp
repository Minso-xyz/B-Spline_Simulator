#include "SimulatorUI.h"
#include "Imgui.h"
#include "BSplineCurve.h"
#include "CSVExporter.h"

SimulatorUI::SimulatorUI(const BSplineCurve& curve)
{
	EditingDegree = curve.Degree;
	EditingControlPoints = curve.ControlPoints;
	EditingControlPointCount = static_cast<int>(curve.ControlPoints.size());
	SampleCount = 50;
}

const char* viewNames[]
{
	"Front",
	"Right",
	"Top",
	"Isometric"
};
static int currentView = 3;

const char* dataExportCSV[]
{
	"Control Points",
	"Knot Vector",
	"Curve Sample Points",
	"Basis Functions"
};
static int currentDataType = 0;


// CSV Export
bool exportAttempted = false;
bool exportSuccess = false;

// false : Apply button not clicked
// true : curve is updated
bool SimulatorUI::Draw(BSplineCurve& curve, Camera& camera, CSVExporter& dataCSV)
{
	bool curveUpdated = false;

	ImGui::Begin("B-Spline Parameters");
	ImGui::Text("             Degree");
	ImGui::SameLine();
	ImGui::InputInt("##Degree", &EditingDegree);

	ImGui::Text("Control Point Count");
	ImGui::SameLine();
	if (ImGui::InputInt("##Control Point Count", &EditingControlPointCount))  // boolean?
	{
		ResizeControlPoints();
	}

	ImGui::Separator();

	for (int i = 0; i < EditingControlPoints.size(); i++)
	{
		ImGui::PushID(i);  // Assign an id
		ImGui::Text("                 P%d", i);  // P0, P1, P2...
		ImGui::SameLine();

		double coordinates[3] =
		{
			EditingControlPoints[i].X,
			EditingControlPoints[i].Y,
			EditingControlPoints[i].Z
		};

		if (ImGui::InputScalarN("##coordinates", ImGuiDataType_Double, coordinates, 3))
		{
			EditingControlPoints[i].X = coordinates[0];
			EditingControlPoints[i].Y = coordinates[1];
			EditingControlPoints[i].Z = coordinates[2];
		}
		ImGui::PopID();
	}
	ImGui::Separator();

	// Validate the input parameters
	bool valid = EditingControlPointCount >= 2 && EditingDegree >= 1 && EditingDegree < EditingControlPointCount;
	if (!valid)
	{
		ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f),
			"Degree must be smaller than "
			"the control point count.");

		ImGui::BeginDisabled();
	}

	// Set camera view
	ImGui::Text("               View");
	ImGui::SameLine();
	if (ImGui::Combo("##View", &currentView, viewNames, IM_ARRAYSIZE(viewNames)))
	{
		switch (currentView)
		{
		case 0:
			camera.SetFrontView();
			break;
		case 1:
			camera.SetRightView();
			break;
		case 2:
			camera.SetTopView();
			break;
		case 3:
			camera.SetIsometricView();
			break;
		}
	}

	

	// Export CSV file
	// Select the type of date to export
	ImGui::Separator();
	ImGui::Spacing();
	ImGui::Text("          Data type");
	ImGui::SameLine();
	ImGui::Combo("##DataType", &currentDataType, dataExportCSV, IM_ARRAYSIZE(dataExportCSV));

	// Sample count input (only for "Curve sample points")
	ImGui::Text("   ");
	if (currentDataType == 2 || currentDataType == 3)
	{
		ImGui::Text("       Sample Count");
		ImGui::SameLine();
		ImGui::InputInt("##Sample Count", &SampleCount);
	}

	// Update Curve
	ImGui::Spacing();
	ImGui::Text("                   ");
	ImGui::SameLine();
	if (ImGui::Button("  Update Curve  "))
	{
		if (valid)
		{
			curve.Degree = EditingDegree;
			curve.ControlPoints = EditingControlPoints;
			curve.Knots = CreateClampedUniformKnots(EditingControlPointCount, EditingDegree);

			curveUpdated = true;
		}
	}

	if (!valid)
	{
		ImGui::EndDisabled();
	}

	// Export CSV button
	ImGui::SameLine();
	ImGui::Text(" ");
	ImGui::SameLine();
	if (ImGui::Button("  Export CSV  "))
	{
		exportAttempted = true;

		switch (currentDataType)
		{
		case 0:
			exportSuccess = CSVExporter::ExportControlPoints(curve, "control_points.csv");
			break;
		case 1:
			exportSuccess = CSVExporter::ExportKnotVector(curve, "knot_vectors.csv");
			break;
		case 2:
			exportSuccess = CSVExporter::ExportCurveSamples(curve, "curve_samples.csv", SampleCount);
			break;
		case 3:
			exportSuccess = CSVExporter::ExportBasisFunctions(curve, "basis_function.csv", SampleCount);
			break;
		}
	}

	

	// Export CSV success/fail message
	ImGui::Separator();
	ImGui::Spacing();
	if (exportAttempted)
	{
		if (exportSuccess)
		{
			ImGui::Text("CSV file has been generated.");
		}
		else
		{
			ImGui::TextColored(ImVec4(1.0f, 0.3f, 0.3f, 1.0f),
				"CSV file generation has been failed.");
		}
	}
	
	ImGui::End();
	return curveUpdated;
}

void SimulatorUI::ResizeControlPoints()
{
	const int minimumCount = 2;
	const int maximumCount = 100;

	if (EditingControlPointCount < minimumCount) EditingControlPointCount = minimumCount;
	if (EditingControlPointCount > maximumCount) EditingControlPointCount = maximumCount;

	int oldCount = static_cast<int>(EditingControlPoints.size());

	EditingControlPoints.resize(EditingControlPointCount);

	for (int i = oldCount; i < EditingControlPointCount; i++)
	{
		EditingControlPoints[i] =
			Point3D(
				static_cast<double>(i) * 10.0,
				0.0,
				0.0);
	}
}

std::vector<double> SimulatorUI::CreateClampedUniformKnots(int controlPointCount, int degree) const
{
	std::vector<double> knots;

	int knotCount = controlPointCount + degree + 1;
	int endMultiplicity = degree + 1;
	int interiorCount = knotCount - 2 * endMultiplicity;

	for (int i = 0; i < endMultiplicity; ++i)
	{
		knots.push_back(0.0);
	}

	for (int i = 1; i <= interiorCount; ++i)
	{
		double knot = static_cast<double>(i) / static_cast<double>(interiorCount + 1);
		knots.push_back(knot);
	}

	for (int i = 0; i < endMultiplicity; ++i)
	{
		knots.push_back(1.0);
	}
	return knots;
}