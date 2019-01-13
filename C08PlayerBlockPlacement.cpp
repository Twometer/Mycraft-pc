#include "C08PlayerBlockPlacement.h"



C08PlayerBlockPlacement::C08PlayerBlockPlacement(POSITION position, char face, char cx, char cy, char cz)
{
	this->position = position;
	this->face = face;
	this->cx = cx;
	this->cy = cy;
	this->cz = cz;
}


C08PlayerBlockPlacement::~C08PlayerBlockPlacement()
{
}

void C08PlayerBlockPlacement::write(McBuffer* buffer) {
	POSITION px = POSITION(position.serialize());
	buffer->putUlong(position.serialize());
	buffer->putBytes(&face, 1);

	buffer->putUshort(0);
	buffer->putBool(false);
	buffer->putUshort(0);
	buffer->putBool(false);

	buffer->putBytes(&cx, 1);
	buffer->putBytes(&cy, 1);
	buffer->putBytes(&cz, 1);
}

void C08PlayerBlockPlacement::read(McBuffer* buffer) {
	// Not implemented
}

int C08PlayerBlockPlacement::getId() {
	return 0x08;
}

