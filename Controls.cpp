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
#include "C06PlayerPosLook.h"

glm::vec3 position = glm::vec3(-100, 270, -100);
glm::vec3 velocityVector = glm::vec3(0, 0, 0);
float yaw = 45.0f;
float pitch = -15.0f;
float speedBase = 5.0f;
float mouseSpeed = 5;
double lastTime;
bool Controls::first;

bool isOnGround;

float gravity = 0.5;
float slipperiness = 0.61;

float sprintTicks = 0.0f;

int last_pos_update = 0;

bool lastInFluid;

float ticks_in_water = 0;

bool ctrl_flying;
bool ctrl_flyingpr;
float ctrl_flyinglast;

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
	physics = true;
}

void Controls::setPosition(float x, float y, float z) {
	position = glm::vec3(x, y, z);
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
		pitch += mouseSpeed * deltaTime * float(OpenGLRenderer::height / 2 - ypos);
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
	bool inFluid = bl == 8 || bl == 9 || bl == 10 || bl == 11;
	if (inFluid && !lastInFluid && velocityVector.y > -0.1) {
		velocityVector.y *= .5;
	}
	if (!inFluid && lastInFluid) {
		velocityVector.y *= 1.6;
	}
	if (inFluid && velocityVector.y < -1.5 * deltaTime) {
		velocityVector.y = -1.5 * deltaTime;
	}
	if (inFluid) ticks_in_water += deltaTime;
	else ticks_in_water = 0;

	lastInFluid = inFluid;


	slipperiness = inFluid ? 0.9 : 0.61;
	if (ctrl_flying) slipperiness = 0.94;
	gravity = inFluid ? 0.1 : 0.5;


	if (focused == GLFW_TRUE && !OpenGLRenderer::chatOpen) {
		if (glfwGetKey(win, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
			sprinting = true;
		}
		speed = sprinting ? speedBase * 1.5 : speedBase;
		if (!ctrl_flying && inFluid) speed *= .4;
		if (ctrl_flying) speed *= 2;
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
			if (isOnGround || (inFluid && ticks_in_water > 0.2) || ctrl_flying) {
				motionVector += glm::vec3(0, inFluid ? 0.8 : 1.6, 0) * deltaTime * speedBase * (ctrl_flying ? 2.0f : 1.0f);
			}
			if (!ctrl_flyingpr) {
				ctrl_flyingpr = true;
				if (clock() - ctrl_flyinglast < 200) {
					ctrl_flying = !ctrl_flying;
				}
				ctrl_flyinglast = clock();
			}
		}
		else ctrl_flyingpr = false;
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
		if (!ctrl_flying)
			velocityVector.y -= deltaTime * gravity;
		else velocityVector.y *= slipperiness;
		velocityVector.z *= slipperiness;
	}

	if (clock() - last_pos_update >= 50) {
		OpenGLRenderer::sendPacket(new C06PlayerPosLook(position.x, position.y, position.z, yaw, pitch, isOnGround));
		last_pos_update = clock();
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