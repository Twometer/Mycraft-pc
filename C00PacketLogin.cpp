#include "stdafx.h"
#include "C00PacketLogin.h"
#include "McBuffer.h"

C00PacketLogin::C00PacketLogin(const char* username)
{
	this->username = username;
}


C00PacketLogin::~C00PacketLogin()
{
}

void C00PacketLogin::write(McBuffer* buffer) {
	buffer->putString(this->username);
}

void C00PacketLogin::read(McBuffer* buffer) {
	// Not implemented
}

int C00PacketLogin::getId() {
	return 0;
}