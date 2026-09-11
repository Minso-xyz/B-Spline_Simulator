#include "Camera.h"
#include <GLFW/glfw3.h>
#include "BoundingBox.h"

Camera::Camera()
{
	Distance = 5.0f;
	Yaw = 45.0f;
	Pitch = -135.0f;
	Zoom = 300.0f;
}

void Camera::SetFrontView()
{
	Yaw = 0.0f;
	Pitch = 0.0f;
}

void Camera::SetBackView()
{
	Yaw = 180.0f;
	Pitch = 0.0f;
}

void Camera::SetLeftView()
{
	Yaw = -90.0f;
	Pitch = 0.0f;
}

void Camera::SetRightView()
{
	Yaw = 90.0f;
	Pitch = 0.0f;
}

void Camera::SetTopView()
{
	Yaw = 0.0f;
	Pitch = -90.0f;
}

void Camera::SetBottomView()
{
	Yaw = 0.0f;
	Pitch = 90.0f;
}

void Camera::SetIsometricView()
{
	Yaw = 45.0f;
	Pitch = -35.264f;
}

void Camera::ApplyProjection(int width, int height)
{
	if (height == 0)
	{
		height = 1;
	}

	float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
	float depthRange = Zoom * 10.0f;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho(
		-Zoom * aspectRatio,
		Zoom * aspectRatio,
		-Zoom,
		Zoom,
		-depthRange,
		depthRange);

	glMatrixMode(GL_MODELVIEW);
}

void Camera::ApplyView()
{
	glLoadIdentity();

	// Define how far is the camera
	glTranslatef(
		0.0f,
		0.0f,
		-Distance);

	// Rotate the view as Isometric
	glRotatef(
		Pitch,
		1.0f,
		0.0f,
		0.0f
	);

	glRotatef(
		Yaw,
		0.0f,
		1.0f,
		0.0f);

	// Define where the camera sees
	glTranslatef(
		static_cast<float>(-Target.X),
		static_cast<float>(-Target.Y),
		static_cast<float>(-Target.Z));
}

void Camera::HandleInput(GLFWwindow* window)
{
	// Zoom-in
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		 Zoom -= Zoom * 0.005f;
	}

	// Zoom-out
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		Zoom += Zoom * 0.005f;
	}

	// Orbit Left
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		Yaw -= Yaw * 0.005f;
	}

	// Orbit Right
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		Yaw += Yaw * 0.005f;
	}

	// Pitch Up
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		Pitch -= Pitch * 0.005f;
	}

	// Pitch Down
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		Pitch += Pitch * 0.005f;
	}
}

void Camera::FitTargetBox(const BoundingBox& boundingBox)
{
	Target = boundingBox.GetCenter();

	double radius = boundingBox.GetModelRadius();

	if (radius < 0.001)
	{
		radius = 1.0;
	}

	Zoom = static_cast<float>(radius * 1.25);
	Distance = static_cast<float>(radius * 3.0);
}

void Camera::HandleMouse(GLFWwindow* window)
{
	double mouseX;
	double mouseY;

	glfwGetCursorPos(window, &mouseX, &mouseY);

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		if (!IsRotating)
		{
			IsRotating = true;
			LastMouseX = mouseX;
			LastMouseY = mouseY;
		}

		double deltaX = mouseX - LastMouseX;
		double deltaY = mouseY - LastMouseY;

		Yaw += static_cast<float>(deltaX) * 0.3f;
		Pitch += static_cast<float>(deltaY) * 0.3f;

		LastMouseX = mouseX;
		LastMouseY = mouseY;
	}
	else
	{
		IsRotating = false;
	}
}
