#pragma once
#include "VertexHandler.h"

class LaddersVertexHandler
	: public VertexHandler
{
	glm::vec3 process(int meta, GLfloat x, GLfloat y, GLfloat z, glm::vec3 worldPos, FACE face)
	{
		if (meta == 2) {
			return rotate90(x, y, z, 0.05f, 0.95f);
		}
		else if (meta == 3) {
			return rotate270(x, y, z, 0.05f, 0.95f);
		}
		else if (meta == 4) {
			glm::vec3 n = rotate90(x, y, z, 0.05f, 0.95f);
			return rotate90(n.x, n.y, n.z, 0.05f, 0.95f);
		}
		return glm::vec3(x, y, z);
	}
};