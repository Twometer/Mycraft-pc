#include "stdafx.h"
#include "C01PacketChat.h"


C01PacketChat::C01PacketChat(const char* message)
{
	this->message = message;
}


C01PacketChat::~C01PacketChat()
{
}

void C01PacketChat::write(McBuffer* buffer) {
	buffer->putString(this->message);
}

void C01PacketChat::read(McBuffer* buffer) {
	// Not implemented
}

int C01PacketChat::getId() {
	return 1;
}