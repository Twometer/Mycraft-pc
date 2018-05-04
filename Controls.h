#pragma once
#include <glm/glm.hpp>
#include "GLFW/glfw3.h"

struct MATRICES {
	glm::mat4 modelviewMatrix;
	glm::mat4 projectionMatrix;
};

class Controls
{

public:
	Controls();
	~Controls();
	MATRICES computeMatrices(GLFWwindow* win);
	glm::vec3 getPosition();
	void setPosition(float x, float y, float z);
	void setPosition(double x, double y, double z);
};

