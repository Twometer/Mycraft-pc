#pragma once
#include "VertexHandler.h"
class RailsVertexHandler
	: public VertexHandler
{
	glm::vec3 process(int meta, GLfloat x, GLfloat y, GLfloat z) {

		if (meta == 5 && z == 1.0f)
			return glm::vec3(x, 1, z);

		if (meta == 4 && z == 0.0f) {
			return glm::vec3(x, 1, z);
		}

		if (meta == 3) {
			if (z == 0.0f) return rotate90(x, 1, z);
			else return rotate90(x, y, z);
		}

		if (meta == 2) {
			if (z == 1.0f) return rotate90(x, 1, z);
			else return rotate90(x, y, z);
		}

		if (meta == 6) {
			return rotate90(x,y,z);
		}

		if (meta == 8) {
			return rotate270(x, y, z);
		}


		if (meta == 9) {
			glm::vec3 n = rotate90(x, y, z);
			n = rotate90(n.x, n.y, n.z);
			return n;
		}


		if (meta >= 8)
			meta -= 8;

		if (meta == 1)
		{
			return rotate90(x, y, z);
		}

		return glm::vec3(x, y, z);
	};
};
