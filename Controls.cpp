#include "Controls.h"
#include <stdio.h>
#include "OpenGLRenderer.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

glm::vec3 position = glm::vec3(-100, 270, -100);
float yaw = 45.0f;
float pitch = -15.0f;
float speed = 100.0f; // 3 units / second
float mouseSpeed = 5;
double lastTime;
int width = 640;
int height = 480;
int first = 1;
Controls::Controls()
{
}


Controls::~Controls()
{
}

void Controls::setPosition(double x, double y, double z) {
	position = glm::vec3(x, y, z);
	std::cout << "New position is " << position.x << endl;
}

void Controls::setPosition(float x, float y, float z) {
	position = glm::vec3(x, y, z);
	std::cout << "New position is " << position.x << endl;
}

glm::mat4 Controls::computeMvpMatrix(GLFWwindow* win) {
	int focused = glfwGetWindowAttrib(win, GLFW_FOCUSED);


	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);
	lastTime = currentTime;

	double xpos, ypos;
	glfwGetCursorPos(win, &xpos, &ypos);

	if (focused == GLFW_TRUE && !first) {
		glfwSetCursorPos(win, width / 2, height / 2);
		yaw += mouseSpeed * deltaTime * float(width / 2 - xpos);
		pitch += mouseSpeed * deltaTime * float(height / 2 - ypos);
		pitch = glm::clamp(pitch, -90.0f, 90.0f);
	}

	if (first) {
		glfwSetCursorPos(win, width / 2, height / 2);
		first = false;
	}

	float yawRadians = glm::radians(yaw);
	float pitchRadians = glm::radians(pitch);
	glm::vec3 direction(
		cos(pitchRadians) * sin(yawRadians),
		sin(pitchRadians),
		cos(pitchRadians) * cos(yawRadians)
	);

	glm::vec3 directionMovement(
		sin(yawRadians),
		0,
		cos(yawRadians)
	);

	glm::vec3 right = glm::vec3(
		sin(yawRadians - 3.14f / 2.0f),
		0,
		cos(yawRadians - 3.14f / 2.0f)
	);

	glm::vec3 up = glm::cross(right, direction);

	if (focused == GLFW_TRUE) {
		if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS) {
			position += directionMovement * deltaTime * speed;
		}
		// Move backward
		if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS) {
			position -= directionMovement * deltaTime * speed;
		}
		// Strafe right
		if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS) {
			position += right * deltaTime * speed;
		}
		// Strafe left
		if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS) {
			position -= right * deltaTime * speed;
		}
		if (glfwGetKey(win, GLFW_KEY_SPACE) == GLFW_PRESS) {
			position += glm::vec3(0, 1.0, 0) * deltaTime * speed;
		}
		if (glfwGetKey(win, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			position -= glm::vec3(0, 1.0, 0) * deltaTime * speed;
		}
	}
	glm::mat4 ModelMatrix(1.0f);
	glm::mat4 ProjectionMatrix = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 2000.0f);
	glm::mat4 ViewMatrix = glm::lookAt(
		position,           // Camera is here
		position + direction, // and looks here : at the same position, plus "direction"
		up                  // Head is up (set to 0,-1,0 to look upside-down)
	);

	glm::mat4 mvpMatrix = ProjectionMatrix * ViewMatrix * ModelMatrix;

	OpenGLRenderer::frustum->extractFrustum(mvpMatrix);

	return mvpMatrix;
}