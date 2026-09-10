#pragma once
#include <GLFW/glfw3.h>
#include "Point3D.h"
#include "BoundingBox.h"

class Camera
{
public:
	float Distance;   // distance between the camera and the model
	float Yaw;   // Right/Left rotation angle
	float Pitch;   // Up/Down rotation angle
	float Zoom;

	Point3D Target;

public:
	Camera();
	void SetFrontView();
	void SetBackView();
	void SetLeftView();
	void SetRightView();
	void SetTopView();
	void SetBottomView();
	void SetIsometricView();

	void ApplyProjection(int width, int height);

	void ApplyView();

	void HandleInput(GLFWwindow* window);

	void FitTargetBox(const BoundingBox& boundingBox);
};