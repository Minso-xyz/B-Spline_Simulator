#include "SimulatorUI.h"
#include "Imgui.h"
#include "BSplineCurve.h"

SimulatorUI::SimulatorUI(const BSplineCurve& curve)
{
	EditingDegree = curve.Degree;
	EditingControlPoints = curve.ControlPoints;
	EditingControlPointCount = static_cast<int>(curve.ControlPoints.size());
}

// false : Apply button not clicked
// true : curve is updated
bool SimulatorUI::Draw(BSplineCurve& curve)
{
	bool curveUpdated = false;

	ImGui::Begin("B-Spline Parameters");
	ImGui::InputInt("Degree", &EditingDegree);

	if (ImGui::InputInt("Control Point Count", &EditingControlPointCount))  // boolean?
	{
		ResizeControlPoints();
	}

	ImGui::Separator();  // ???


	for (int i = 0; i < EditingControlPoints.size(); i++)
	{
		ImGui::PushID(i);  // ???
		ImGui::Text("P%d", i);  // ???
		ImGui::SameLine();  // ????

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

	if (ImGui::Button("Apply Curve"))
	{
		curve.Degree = EditingDegree;
		curve.ControlPoints = EditingControlPoints;
		curve.Knots = CreateClampedUniformKnots(EditingControlPointCount, EditingDegree);

		curveUpdated = true;
	}

	if (!valid)
	{
		ImGui::EndDisabled();
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