#pragma once
#include "BoundingBoxProvider.h"
#include "OpenGLRenderer.h"
#include "DoorState.h"

class DoorBoundingBoxProvider
	: public BoundingBoxProvider
{

	AABB getBoundingBox(int x, int y, int z, int meta)
	{
		DOORSTATE state(meta);
		if (state.half == UPPER)
			state = DOORSTATE(OpenGLRenderer::world->getMeta(x, y - 1, z));

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

		if (state.facing == POSX)
			return AABB(glm::vec3(x, y, z), glm::vec3(x, y, z)).expand(0.2f, 1.0f, 1.0f);
		if (state.facing == POSZ)
			return AABB(glm::vec3(x, y, z), glm::vec3(x, y, z)).expand(1.0f, 1.0f, 0.2f);
		if (state.facing == NEGX)
			return AABB(glm::vec3(x + 0.8, y, z), glm::vec3(x + 0.8, y, z)).expand(0.2f, 1.0f, 1.0f);
		if (state.facing == NEGZ)
			return AABB(glm::vec3(x, y, z + 0.8), glm::vec3(x, y, z + 0.8)).expand(1.0f, 1.0f, 0.2f);

		return AABB(glm::vec3(x, y, z), glm::vec3(x, y, z)).expand(1.0f, 1.0f, 1.0f);
	}

};