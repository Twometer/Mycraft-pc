#pragma once
#include "IPacket.h"
class C00PacketLogin :
	public IPacket
{
private:
	const char* username;
public:
	C00PacketLogin(const char* username);
	~C00PacketLogin();
	virtual void write(McBuffer* buffer);
	virtual void read(McBuffer* buffer);
	virtual int getId();
};

