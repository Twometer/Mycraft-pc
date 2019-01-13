#pragma once
#include "VertexHandler.h"
#include "DoorState.h"
#include "OpenGLRenderer.h"

class DoorVertexHandler
	: public VertexHandler
{

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
			z = 0.1f;
		if (state.facing == POSX && x == 1.0f)
			x = 0.1f;
		if (state.facing == NEGZ && z == 0.0f)
			z = 0.9f;
		if (state.facing == NEGX && x == 0.0f)
			x = 0.9f;

		return glm::vec3(x, y, z);
	}

};