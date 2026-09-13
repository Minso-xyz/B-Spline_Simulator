#include "SimulatorUI.h"
#include "Imgui.h"
#include "BSplineCurve.h"
#include "CSVExporter.h"
#include "BSplineSurface.h"
#include "SurfacePreset.h"

SimulatorUI::SimulatorUI(const BSplineCurve& curve)
{
	EditingDegree = curve.Degree;
	EditingControlPoints = curve.ControlPoints;
	EditingControlPointCount = static_cast<int>(curve.ControlPoints.size());
	SampleCount = 50;
	SampleCountSurface = 30;

	// B-Spline surface visualization
	ShowCurve = true;
	ShowSurface = true;
	ShowControlNet = true;
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

const char* surfacePresets[]
{
	"Flat",
	"Dome",
	"Wave",
	"Gaussian"
};
static int currentPreset = 3;

const char* degreeOptions[]
{
	"1",
	"2",
	"3"
};
static int degreeIndex = 2;

// CSV Export
bool exportAttempted = false;
bool exportSuccess = false;

// false : Apply button not clicked
// true : curve is updated
bool SimulatorUI::Draw(BSplineCurve& curve, BSplineSurface& surface, Camera& camera, CSVExporter& dataCSV)
{
	bool curveUpdated = false;

	ImGui::Begin("B-Spline Simulator");
	if (ImGui::BeginTabBar("SimulatorTabs"))
	{
		if (ImGui::BeginTabItem("Curve"))
		{
			// B=Spline Curve  UI
			ImGui::Text("             Degree");
			ImGui::SameLine();
			ImGui::InputInt("##Degree", &EditingDegree);

			ImGui::Text("Control Point Count");
			ImGui::SameLine();
			if (ImGui::InputInt("##Control Point Count", &EditingControlPointCount))
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

			// Show Curve option
			ImGui::Checkbox("Show Curve", &ShowCurve);

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
			ImGui::EndTabItem();
		}

		// B-Spline Surface  UI
		int maxDegreeU = static_cast<int>(surface.ControlNet.size()) - 1;
		int maxDegreeV = static_cast<int>(surface.ControlNet[0].size()) - 1;
		
		if (ImGui::BeginTabItem("Surface"))
		{
			
			if (ImGui::BeginTabBar("SurfaceTabs"))
			{
				
				if (ImGui::BeginTabItem("Preset"))
				{
					// Select the preset
					ImGui::Text("    Preset");
					ImGui::SameLine();
					if (ImGui::Combo("##Preset", &currentPreset, surfacePresets, IM_ARRAYSIZE(surfacePresets)))
					{
						switch (currentPreset)
						{
						case 0: // Flat
							ApplyFlatPreset(surface);
							break;

						case 1:  // Dome
							ApplyDomePreset(surface);
							break;

						case 2:  // Wave
							ApplyWavePreset(surface);
							break;
						case 3:
							ApplyGaussianPreset(surface);
							break;
						}
					}

					ImGui::Text("       ");
					ImGui::Text("       ");

					ImGui::EndTabItem();
				}
				

				// Parameters
				if (ImGui::BeginTabItem("Parameters"))
				{
					// Degree U
					ImGui::Text("    Degree U");
					ImGui::SameLine();
					//ImGui::InputInt("##Degree U", & surface.DegreeU);
					//surface.DegreeU = std::max(1,std::min(surface.DegreeU,maxDegreeU));
					if (ImGui::Combo("##Degree U", &degreeIndex, degreeOptions, IM_ARRAYSIZE(degreeOptions)))
					{
						surface.DegreeU = degreeIndex;
						switch (surface.DegreeU)
						{
						case 1:
							surface.KnotsU =
							{
							0,0,
							0.333,
							0.666,
							1,1
							};
							break;

						case 2:
							surface.KnotsU =
							{
							0,0,0,
							0.5,
							1,1,1
							};
							break;

						case 3:
							surface.KnotsU =
							{
							0,0,0,0,
							1,1,1,1
							};
							break;
						}
					}

					// Degree V
					ImGui::Text("    Degree V");
					ImGui::SameLine();
					//ImGui::InputInt("##Degree V", &surface.DegreeV);
					//surface.DegreeV =std::max(1,std::min(surface.DegreeV,maxDegreeV));
					if (ImGui::Combo("##Degree V", &degreeIndex, degreeOptions, IM_ARRAYSIZE(degreeOptions)))
					{
						surface.DegreeV = degreeIndex;
						switch (surface.DegreeV)
						{
						case 1:
							surface.KnotsV =
							{
							0,0,
							0.333,
							0.666,
							1,1
							};
							break;

						case 2:
							surface.KnotsV =
							{
							0,0,0,
							0.5,
							1,1,1
							};
							break;

						case 3:
							surface.KnotsV =
							{
							0,0,0,0,
							1,1,1,1
							};
							break;
						}
					}

					// Sample Count
					ImGui::Text("Sample Count");
					ImGui::SameLine();
					ImGui::InputInt("##Sample Count", &SampleCountSurface);

					ImGui::EndTabItem();
				}

				// Visualization options : Show surface / Control net
				if (ImGui::BeginTabItem("Visualization"))
				{
					ImGui::Text("       ");
					ImGui::SameLine();
					ImGui::Checkbox("Show Surface", &ShowSurface);
					ImGui::Text("       ");
					ImGui::SameLine();
					ImGui::Checkbox("Show Control Net",&ShowControlNet);

					ImGui::Spacing();
					// Set camera view
					ImGui::Text("      View");
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

					ImGui::EndTabItem();
				}

				//// Control Net
				//if (ImGui::BeginTabItem("Control Net"))
				//{
				//	if (ImGui::TreeNode("Control Net"))
				//	{

				//	}

				//	ImGui::EndTabItem();
				//}

				

				ImGui::EndTabBar();
			}

			ImGui::EndTabItem();

			

			/*ImGui::Spacing();
			ImGui::Separator();
			ImGui::Button("  Update Surface  ");*/
		}

		ImGui::EndTabBar();
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