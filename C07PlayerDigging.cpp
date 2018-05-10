#include "C07PlayerDigging.h"
#include <iostream>



C07PlayerDigging::C07PlayerDigging(ACTION status, POSITION pos, char face)
{
	this->status = status;
	this->position = pos;
	this->face = face;
}


C07PlayerDigging::~C07PlayerDigging()
{
}

void C07PlayerDigging::write(McBuffer* buffer) {
	POSITION px = POSITION(position.serialize());
	buffer->putVarInt(status);
	buffer->putUlong(position.serialize());
	buffer->putBytes(&face, 1);
}

void C07PlayerDigging::read(McBuffer* buffer) {
	// Not implemented
}

int C07PlayerDigging::getId() {
	return 0x07;
}
