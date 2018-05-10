#pragma once
#include "RaycastResult.h"
class Raycast
{
public:
	Raycast();
	~Raycast();
	RAYCAST_RESULT cast(float yaw, float pitch);
};

