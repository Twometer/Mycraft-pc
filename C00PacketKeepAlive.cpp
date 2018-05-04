#include "stdafx.h"
#include "C00PacketKeepAlive.h"


C00PacketKeepAlive::C00PacketKeepAlive(int id)
{
	this->id = id;
}


C00PacketKeepAlive::~C00PacketKeepAlive()
{
}

void C00PacketKeepAlive::write(McBuffer* buffer) {
	buffer->putVarInt(this->id);
}

void C00PacketKeepAlive::read(McBuffer* buffer) {
	// Not implemented
}

int C00PacketKeepAlive::getId() {
	return 0;
}
