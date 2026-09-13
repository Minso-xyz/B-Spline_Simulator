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

	// Gaussian
	BSplineSurface surface;

	surface.DegreeU = 3;
	surface.DegreeV = 3;

	surface.KnotsU =
	{
	0.0, 0.0, 0.0, 0.0,
	1.0, 1.0, 1.0, 1.0
	};

	surface.KnotsV =
	{
	0.0, 0.0, 0.0, 0.0,
	1.0, 1.0, 1.0, 1.0
	};

	surface.ControlNet =
	{
	{
	Point3D(0, 15, 0),
	Point3D(10, 18, 0),
	Point3D(20, 18, 0),
	Point3D(30, 15, 0)
	},

	{
	Point3D(0, 22, 10),
	Point3D(10, 35, 10),
	Point3D(20, 35, 10),
	Point3D(30, 22, 10)
	},

	{
	Point3D(0, 22, 20),
	Point3D(10, 50, 20),
	Point3D(20, 50, 20),
	Point3D(30, 22, 20)
	},

	{
	Point3D(0, 15, 30),
	Point3D(10, 18, 30),
	Point3D(20, 18, 30),
	Point3D(30, 15, 30)
	}
	
	};

	//// wave
	//BSplineSurface surface;

	//surface.DegreeU = 3;
	//surface.DegreeV = 3;

	//surface.KnotsU =
	//{
	//0.0, 0.0, 0.0, 0.0,
	//1.0, 1.0, 1.0, 1.0
	//};

	//surface.KnotsV =
	//{
	//0.0, 0.0, 0.0, 0.0,
	//1.0, 1.0, 1.0, 1.0
	//};

	//surface.ControlNet =
	//{
	//{
	//Point3D(0, 20, 0),
	//Point3D(10, 40, 0),
	//Point3D(20, 15, 0),
	//Point3D(30, 35, 0)
	//},

	//{
	//Point3D(0, 35, 10),
	//Point3D(10, 55, 10),
	//Point3D(20, 30, 10),
	//Point3D(30, 45, 10)
	//},

	//{
	//Point3D(0, 15, 20),
	//Point3D(10, 35, 20),
	//Point3D(20, 10, 20),
	//Point3D(30, 25, 20)
	//},

	//{
	//Point3D(0, 30, 30),
	//Point3D(10, 50, 30),
	//Point3D(20, 25, 30),
	//Point3D(30, 40, 30)
	//}

	//};


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

		bool curveUpdated = ui.Draw(curve, surface, camera, CSVExporter);

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

		ImGui::Render();
		ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

		renderer.DrawCoordinateAxis();   // Draw the coordinate axis

		// Draw B-Spline Curve
		if (ui.ShowCurve)
		{
			renderer.DrawBSplineCurve(curve);
			renderer.DrawControlPolygon(curve.ControlPoints);
		}
		
		// Draw B-Spline Surface
		if (ui.ShowSurface)
		{
			renderer.DrawBSplineSurface(surface, ui.SampleCountSurface);
		}

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

