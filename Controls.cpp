#include "Controls.h"
#include <stdio.h>
#include "OpenGLRenderer.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "AABB.h"
#include "Settings.h"

glm::vec3 position = glm::vec3(-100, 270, -100);
glm::vec3 velocityVector = glm::vec3(0, 0, 0);
float yaw = 45.0f;
float pitch = -15.0f;
float speedBase = 5.0f;
float mouseSpeed = 5;
double lastTime;
int first = 1;

bool isOnGround;

float gravity = 0.5;
float slipperiness = 0.61;

float sprintTicks = 0.0f;

bool lastWater;

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

glm::vec2 Controls::getRotation() {
	return glm::vec2(yaw, pitch);
}

bool physics = false;

void Controls::setPosition(double x, double y, double z) {
	position = glm::vec3(x, y, z);
	std::cout << "New position is " << position.x << endl;
	physics = true;
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

	if (focused == GLFW_TRUE && !first && !OpenGLRenderer::chatOpen) {
		glfwSetCursorPos(win, OpenGLRenderer::width / 2, OpenGLRenderer::height / 2);
		yaw += mouseSpeed * deltaTime * float(OpenGLRenderer::width / 2 - xpos);
		pitch += mouseSpeed * deltaTime * float(OpenGLRenderer::height  / 2 - ypos);
		pitch = glm::clamp(pitch, -90.0f, 90.0f);
	}

	if (first) {
		glfwSetCursorPos(win, OpenGLRenderer::width / 2, OpenGLRenderer::height / 2);
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

	bool sprinting = false;
	float speed = 0;

	glm::vec3 x = getEyePosition();
	char bl = OpenGLRenderer::world->getBlock(floor(x.x), floor(x.y - 1), floor(x.z));
	bool inWater = bl == 8 || bl == 9;
	if (inWater && !lastWater) {
		velocityVector.y *= .3;
	}
	lastWater = inWater;


	slipperiness = inWater ? 0.9 : 0.61;
	gravity = inWater ? 0.1 : 0.5;


	if (focused == GLFW_TRUE && !OpenGLRenderer::chatOpen) {
		if (glfwGetKey(win, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
			sprinting = true;
		}
		speed = sprinting ? speedBase * 1.5 : speedBase;
		if (inWater) speed *= .4;
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
			if (isOnGround || inWater) {
				motionVector += glm::vec3(0, inWater ? 0.8 : 1.6, 0) * deltaTime * speedBase;
			}
		}
		if (glfwGetKey(win, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
			motionVector -= glm::vec3(0, 1.0, 0) * deltaTime * speed;
		}
	}


	if (motionVector.x != 0) velocityVector.x = motionVector.x;
	if (motionVector.y != 0) velocityVector.y = motionVector.y;
	if (motionVector.z != 0) velocityVector.z = motionVector.z;

	AABB myAABB = AABB(position - glm::vec3(0.33, 0, 0.33), position + glm::vec3(0.33, 1.9, 0.33));
	vector<AABB> v = OpenGLRenderer::world->getCubes(floor(position.x), floor(position.y), floor(position.z), 6);
	double yaOrg = velocityVector.y;
	for (int i = 0; i < v.size(); i++) {
		velocityVector.y = v.at(i).clipYCollide(myAABB, velocityVector.y);
	}
	myAABB.move(0, velocityVector.y, 0);
	for (int i = 0; i < v.size(); i++) {
		velocityVector.x = v.at(i).clipXCollide(myAABB, velocityVector.x);
	}
	myAABB.move(velocityVector.x, 0, 0);
	for (int i = 0; i < v.size(); i++) {
		velocityVector.z = v.at(i).clipZCollide(myAABB, velocityVector.z);
	}
	myAABB.move(0, 0, velocityVector.z);

	isOnGround = velocityVector.y != yaOrg && yaOrg < 0.0;

	position = glm::vec3((myAABB.p0.x + myAABB.p1.x) / 2, myAABB.p0.y, (myAABB.p0.z + myAABB.p1.z) / 2);

	if (physics) {
		velocityVector.x *= slipperiness;
		velocityVector.y -= deltaTime * gravity;
		velocityVector.z *= slipperiness;
	}

	glm::mat4 ModelMatrix(1.0f);
	float fovDiff = (Settings::FOV * 1.2f) - Settings::FOV;

	if (sprinting && sprintTicks < 1.0) sprintTicks += deltaTime * 8;
	else if (!sprinting && sprintTicks > 0.0) sprintTicks -= deltaTime * 8;

	float add = fovDiff * sprintTicks;

	glm::mat4 ProjectionMatrix = glm::perspective(glm::radians(Settings::FOV + (add)), (float)OpenGLRenderer::width / (float)OpenGLRenderer::height, 0.1f, 2000.0f);
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