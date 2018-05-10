#include "C06PlayerPosLook.h"
#include <iostream>



C06PlayerPosLook::C06PlayerPosLook(double x, double y, double z, float yaw, float pitch, bool onGround)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->yaw = yaw;
	this->pitch = pitch;
	this->onGround = onGround;
}


C06PlayerPosLook::~C06PlayerPosLook()
{
}

void C06PlayerPosLook::write(McBuffer* buffer) {
	buffer->putDouble(x);
	buffer->putDouble(y);
	buffer->putDouble(z);
	buffer->putFloat(-yaw);
	buffer->putFloat(-pitch);
	buffer->putBool(onGround);
}

void C06PlayerPosLook::read(McBuffer* buffer) {
	// Not implemented
}

int C06PlayerPosLook::getId() {
	return 0x06;
}