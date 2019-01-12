#pragma once
#include <glm/common.hpp>
#include "GLFW/glfw3.h"
class VertexHandler {

public:
	virtual glm::vec3 process(int meta, GLfloat x, GLfloat, GLfloat z) = 0;

protected:

	const glm::vec3 rotate90(GLfloat x, GLfloat y, GLfloat z, GLfloat v0 = 0.0f, GLfloat v1 = 1.0f) {
		if (x == v0 && z == v0)
			return glm::vec3(v0, y, v1);
		if (x == v0 && z == v1)
			return glm::vec3(v1, y, v1);
		if (x == v1 && z == v1)
			return glm::vec3(v1, y, v0);
		if (x == v1 && z == v0)
			return glm::vec3(v0, y, v0);
		return glm::vec3(x, y, z);
	};

	const glm::vec3 rotate180(GLfloat x, GLfloat y, GLfloat z, GLfloat v0 = 0.0f, GLfloat v1 = 1.0f) {
		if (x == v0 && z == v0)
			return glm::vec3(v1, y, v1);
		if (x == v0 && z == v1)
			return glm::vec3(v0, y, v1);
		if (x == v1 && z == v1)
			return glm::vec3(v0, y, v0);
		if (x == v1 && z == v0)
			return glm::vec3(v1, y, v0);
		return glm::vec3(x, y, z);
	};

	const glm::vec3 rotate270(GLfloat x, GLfloat y, GLfloat z, GLfloat v0 = 0.0f, GLfloat v1 = 1.0f) {
		if (x == v0 && z == v0)
			return glm::vec3(v1, y, v0);
		if (x == v1 && z == v0)
			return glm::vec3(v1, y, v1);
		if (x == v1 && z == v1)
			return glm::vec3(v0, y, v1);
		if (x == v0 && z == v1)
			return glm::vec3(v0, y, v0);
		return glm::vec3(x, y, z);
	};

};