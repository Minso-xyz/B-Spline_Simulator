#include "SimulatorUI.h"
#include "Imgui.h"

void SimulatorUI::Draw()
{
	ImGui::Begin("B-Spline");
	ImGui::Text("B-Spline Test!");
	ImGui::InputInt("Degree", &Degree);
	ImGui::End();
}