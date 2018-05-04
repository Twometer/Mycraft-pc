#include "stdafx.h"
#include "C00PacketHandshake.h"
#include "McBuffer.h"

C00PacketHandshake::C00PacketHandshake(int protocolVersion, const char* hostname, unsigned short port, int nextState)
{
	this->protocolVersion = protocolVersion;
	this->hostname = hostname;
	this->port = port;
	this->nextState = nextState;
}


C00PacketHandshake::~C00PacketHandshake()
{
}

void C00PacketHandshake::write(McBuffer* buffer) {
	buffer->putVarInt(this->protocolVersion);
	buffer->putString(this->hostname);
	buffer->putUshort(this->port);
	buffer->putVarInt(this->nextState);
}

void C00PacketHandshake::read(McBuffer* buffer) {
	// Not implemented
}

int C00PacketHandshake::getId() {
	return 0;
}