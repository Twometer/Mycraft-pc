#include "Controls.h"
#include <stdio.h>
#include "OpenGLRenderer.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "AABB.h"

glm::vec3 position = glm::vec3(-100, 270, -100);
float yaw = 45.0f;
float pitch = -15.0f;
float speed = 5.0f;
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

glm::vec3 Controls::getPosition() {
	return position;
}

glm::vec3 Controls::getEyePosition() {
	return position + glm::vec3(0, 1.73, 0);
}

void Controls::setPosition(double x, double y, double z) {
	position = glm::vec3(x, y, z);
	std::cout << "New position is " << position.x << endl;
}

void Controls::setPosition(float x, float y, float z) {
	position = glm::vec3(x, y, z);
	std::cout << "New position is " << position.x << endl;
}

MATRICES Controls::computeMatrices(GLFWwindow* win) {

	glm::vec3 eyePosition = getEyePosition();

	MATRICES matrices = MATRICES();

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

	glm::vec3 motionVector = glm::vec3(0, 0, 0);

	if (focused == GLFW_TRUE) {
		if (glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS) {
			motionVector += directionMovement * deltaTime * speed;
		}
		// Move backward
		if (glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS) {
			motionVector -= directionMovement * deltaTime * speed;
		}
		// Strafe right
		if (glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS) {
			motionVector += right * deltaTime * speed;
		}
		// Strafe left
		if (glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS) {
			motionVector -= right * deltaTime * speed;
		}
		if (glfwGetKey(win, GLFW_KEY_SPACE) == GLFW_PRESS) {
			motionVector += glm::vec3(0, 1.0, 0) * deltaTime * speed;
		}
		if (glfwGetKey(win, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			motionVector -= glm::vec3(0, 1.0, 0) * deltaTime * speed;
		}
	}

	AABB myAABB = AABB(position - glm::vec3(0.33, 0, 0.33), position + glm::vec3(0.33, 1.9, 0.33));
	vector<AABB> v = OpenGLRenderer::world->getCubes(floor(position.x), floor(position.y), floor(position.z), 6);
	for (int i = 0; i < v.size(); i++) {
		motionVector.y = v.at(i).clipYCollide(myAABB, motionVector.y);
	}
	myAABB.move(0, motionVector.y, 0);
	for (int i = 0; i < v.size(); i++) {
		motionVector.x = v.at(i).clipXCollide(myAABB, motionVector.x);
	}
	myAABB.move(motionVector.x, 0, 0);
	for (int i = 0; i < v.size(); i++) {
		motionVector.z = v.at(i).clipZCollide(myAABB, motionVector.z);
	}
	myAABB.move(0, 0, motionVector.z);

	position = glm::vec3((myAABB.p0.x + myAABB.p1.x) / 2, myAABB.p0.y, (myAABB.p0.z + myAABB.p1.z) / 2);

	glm::mat4 ModelMatrix(1.0f);
	glm::mat4 ProjectionMatrix = glm::perspective(glm::radians(65.0f), 4.0f / 3.0f, 0.1f, 2000.0f);
	glm::mat4 ViewMatrix = glm::lookAt(
		eyePosition,           // Camera is here
		eyePosition + direction, // and looks here : at the same position, plus "direction"
		up                  // Head is up (set to 0,-1,0 to look upside-down)
	);

	matrices.modelviewMatrix = ViewMatrix * ModelMatrix;
	matrices.projectionMatrix = ProjectionMatrix;
	matrices.viewMatrix = ViewMatrix;

	OpenGLRenderer::frustum->extractFrustum(matrices.projectionMatrix * matrices.modelviewMatrix);

	return matrices;
}