#include <iostream>
#include <GLFW/glfw3.h>
#include "Renderer.h"
#include <vector>
#include "Camera.h"
#include "BSplineCurve.h"

int main()
{
	if (!glfwInit())
	{
		return -1;
	}

	GLFWwindow* window = glfwCreateWindow(800, 600, "STEP Viewer", nullptr, nullptr);  // create the window

	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glEnable(GL_DEPTH_TEST);

	Camera camera;
	Renderer renderer;

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

	camera.SetIsometricView();   // Apply the iso metric view

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

		renderer.DrawCoordinateAxis();   // Draw the coordinate axis
		renderer.DrawBSplineCurve(curve);   // Draw B-Spline Curve

		glfwSwapBuffers(window);
		glfwPollEvents();   // handle the mouse/keyboard inputs
		camera.HandleInput(window);
	}
	glfwTerminate();

	return 0;
}