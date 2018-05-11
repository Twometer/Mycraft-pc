#include "C16Respawn.h"



C16Respawn::C16Respawn()
{
}


C16Respawn::~C16Respawn()
{
}

void C16Respawn::write(McBuffer* buffer) {
	buffer->putVarInt(0);
}

void C16Respawn::read(McBuffer* buffer) {
	// Not implemented
}

int C16Respawn::getId() {
	return 0x16;
}