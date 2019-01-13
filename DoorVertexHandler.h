#pragma once
#include "VertexHandler.h"
#include "DoorState.h"
#include "OpenGLRenderer.h"

class DoorVertexHandler
	: public VertexHandler
{
	Facing facing;

	glm::vec2 processTex(GLfloat u, GLfloat v, FACE face) {

		if (facing == POSX || facing == NEGX) {
			if (face == ZNEG || face == ZPOS)
				return glm::vec2(u * 0.1875, v);
		}
		if (facing == POSZ || facing == NEGZ) {
			if (face == XNEG || face == XPOS)
				return glm::vec2(u * 0.1875, v);
		}
		if (face == YPOS) {
			if (facing == POSZ || facing == NEGZ)
				return glm::vec2(u, v * 0.1875);
			else
				return glm::vec2(u * 0.1875, v);
		}
		return glm::vec2(u, v);
	};

	glm::vec3 process(int meta, GLfloat x, GLfloat y, GLfloat z, glm::vec3 worldPos, FACE face)
	{
		DOORSTATE state(meta);
		if (state.half == UPPER) {
			state = DOORSTATE(OpenGLRenderer::world->getMeta(worldPos.x, worldPos.y - 1, worldPos.z));
		}

		if (state.open) {
			if (state.facing == POSX)
				state.facing = POSZ;
			else if (state.facing == POSZ)
				state.facing = NEGX;
			else if (state.facing == NEGX)
				state.facing = NEGZ;
			else if (state.facing == NEGZ)
				state.facing = POSX;
		}

		if (state.facing == POSZ && z == 1.0f)
			z = 0.2f;
		if (state.facing == POSX && x == 1.0f)
			x = 0.2f;
		if (state.facing == NEGZ && z == 0.0f)
			z = 0.8f;
		if (state.facing == NEGX && x == 0.0f)
			x = 0.8f;

		this->facing = state.facing;

		return glm::vec3(x, y, z);
	};

};