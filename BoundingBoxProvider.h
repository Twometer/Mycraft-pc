#pragma once
#include "AABB.h"
class BoundingBoxProvider 
{
public:
	virtual AABB getBoundingBox(int x, int y, int z, int meta) = 0;
};