#include "Raycast.h"
#include <glm/glm.hpp>
#include "Controls.h"
#include "OpenGLRenderer.h"
#include "BlockRegistry.h"

using namespace glm;

Raycast::Raycast()
{
}


Raycast::~Raycast()
{
}

RAYCAST_RESULT Raycast::cast(float yaw, float pitch) {

	float range = 4.5;

	RAYCAST_RESULT result = RAYCAST_RESULT();

	float precision = 0.005f;
	float pitchMod = sin(radians(90 - pitch));

	vec3 mov = vec3(-sin(radians(-yaw))* pitchMod * precision, -sin(radians(-pitch))* precision, cos(radians(-yaw)) * pitchMod * precision);
	vec3 pos = OpenGLRenderer::controls->getEyePosition();

	World world = *OpenGLRenderer::world;

	FACE face = NO_RESULT;

	for (int i = 0; i <= range * (1 / precision); i++) {
		pos += mov;
		int fx = (int)floor(pos.x);
		int fy = (int)floor(pos.y);
		int fz = (int)floor(pos.z);
		unsigned char bid = world.getBlock(fx, fy, fz);
		Block* block = BlockRegistry::getBlock(bid);
		if (bid != 0 && block->rendererType != Fluid) {
			float xd = round(pos.x - fx);
			float yd = round(pos.y - fy);
			float zd = round(pos.z - fz);
			if (xd == 0) xd = -1;
			if (yd == 0) yd = -1;
			if (zd == 0) zd = -1;

			if (yd > block->blockHeight)
				continue;

			if (xd == 1)
				face = XPOS;
			if (xd == -1)
				face = XNEG;
			if (yd == 1)
				face = YPOS;
			if (yd == -1)
				face = YNEG;
			if (zd == 1)
				face = ZPOS;
			if (zd == -1)
				face = ZNEG;

			result.face = face;
			result.blockX = fx;
			result.blockY = fy;
			result.blockZ = fz;
			result.blockId = bid;
			break;
		}
	}

	return result;
}