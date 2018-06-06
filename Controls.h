#pragma once
#include <glm/glm.hpp>
#include "GLFW/glfw3.h"

struct MATRICES {
	glm::mat4 viewMatrix;
	glm::mat4 modelviewMatrix;
	glm::mat4 projectionMatrix;
};

class Controls
{

public:
	static bool first;
	static float near_plane;
	static float far_plane;
	Controls();
	~Controls();
	MATRICES computeMatrices(GLFWwindow* win);
	glm::vec3 getPosition();
	glm::vec3 getEyePosition();
	glm::vec2 getRotation();
	void setPosition(float x, float y, float z);
	void setPosition(double x, double y, double z);
};

