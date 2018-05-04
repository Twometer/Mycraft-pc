#pragma once
#include <glm/glm.hpp>
#include "GLFW/glfw3.h"
class Controls
{

public:
	Controls();
	~Controls();
	glm::mat4 computeMvpMatrix(GLFWwindow* win);
	glm::vec3 getPosition();
	void setPosition(float x, float y, float z);
	void setPosition(double x, double y, double z);
};

