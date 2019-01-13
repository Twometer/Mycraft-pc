#pragma once
#include "BoundingBoxProvider.h"

class CactusBoundingBoxProvider
	: public BoundingBoxProvider
{

	AABB getBoundingBox(int x, int y, int z, int meta)
	{
		return AABB(glm::vec3(x + 0.0625, y, z + 0.0625), glm::vec3(x + 0.0625, y, z + 0.0625)).expand(1.0f - 0.0625 * 2, 1.0f, 1.0f - 0.0625 * 2);
	}

};