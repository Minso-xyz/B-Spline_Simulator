#include <iostream>
#include <GLFW/glfw3.h>
#include "Renderer.h"
#include <vector>
#include "Camera.h"
#include "BSplineCurve.h"
#include "imgui.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl2.h"
#include "SimulatorUI.h"
#include "CSVExporter.h"
#include "BSplineSurface.h"

Camera* gCamera = nullptr;

void ScrollCallback(GLFWwindow* window,double xOffset,double yOffset);

int main()
{
	if (!glfwInit())
	{
		return -1;
	}

	GLFWwindow* window = glfwCreateWindow(1400, 800, "B-Spline Simulator", nullptr, nullptr);  // create the window

	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glEnable(GL_DEPTH_TEST);
	glfwSetScrollCallback(window, ScrollCallback);

	Camera camera;
	gCamera = &camera;
	Renderer renderer;
	CSVExporter CSVExporter;
	
	// B-Spline Curve test
	BSplineCurve curve(
		{
			Point3D(0,0,0),
			Point3D(10,20,0),
			Point3D(20,20,0),
			Point3D(30,0,0)
		},
		{
			0,0,0,0,
			1,1,1,1
		},
		3
	);

	// B-Spline Surface test
	BSplineSurface surface;

	surface.DegreeU = 3;
	surface.DegreeV = 3;

	surface.KnotsU =
	{
	0,0,0,0,
	1,1,1,1
	};

	surface.KnotsV =
	{
	0,0,0,0,
	1,1,1,1
	};

	surface.ControlNet =
	{
	{
	Point3D(0,0,0),
	Point3D(10,0,0),
	Point3D(20,0,0),
	Point3D(30,0,0)
	},

	{
	Point3D(0,10,0),
	Point3D(10,10,15),
	Point3D(20,10,15),
	Point3D(30,10,0)
	},

	{
	Point3D(0,20,0),
	Point3D(10,20,15),
	Point3D(20,20,15),
	Point3D(30,20,0)
	},

	{
	Point3D(0,30,0),
	Point3D(10,30,0),
	Point3D(20,30,0),
	Point3D(30,30,0)
	}
	};

	SimulatorUI ui(curve);

	camera.SetIsometricView();   // Apply the iso metric view

	// Add IMGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window,true);
	ImGui_ImplOpenGL2_Init();

	while (!glfwWindowShouldClose(window))
	{
		int width;
		int height;

		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		camera.ApplyProjection(width, height);
		camera.ApplyView();

		// IMGUI
		ImGui_ImplOpenGL2_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		bool curveUpdated = ui.Draw(curve, camera, CSVExporter);

		ImGuiIO& io = ImGui::GetIO();

		// Rotate the view only when the house is not in other use (ImGui interfaces)
		if (!io.WantCaptureMouse)
		{
			camera.HandleMouse(window);
		}

		if (curveUpdated)
		{
			BoundingBox box = BoundingBox::CalculateBoundingBox(curve.ControlPoints);
			camera.FitTargetBox(box);
		}
		renderer.DrawBSplineCurve(curve);
		renderer.DrawControlPolygon(curve.ControlPoints);

		ImGui::Render();
		ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

		renderer.DrawCoordinateAxis();   // Draw the coordinate axis
		renderer.DrawBSplineCurve(curve);   // Draw B-Spline Curve
		renderer.DrawControlPolygon(curve.ControlPoints);

		glfwSwapBuffers(window);
		glfwPollEvents();   // handle the mouse/keyboard inputs
		camera.HandleInput(window);
	}
	glfwTerminate();

	return 0;
}

void ScrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	// abort if the mouse is in other use (ImGui)
	if (ImGui::GetIO().WantCaptureMouse)
	{
		return;
	}

	if (gCamera)
	{
		gCamera->HandleScroll(yOffset);  // only yOffset is used (Up-down mouse wheel)
	}
}

