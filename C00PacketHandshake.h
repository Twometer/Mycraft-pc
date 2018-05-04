#pragma once
#include "IPacket.h"
class C00PacketHandshake :
	public IPacket
{
private:
	int protocolVersion;
	const char* hostname;
	unsigned short port;
	int nextState;
public:
	C00PacketHandshake(int protocolVersion, const char* hostname, unsigned short port, int nextState);
	~C00PacketHandshake();
	virtual void write(McBuffer* buffer);
	virtual void read(McBuffer* buffer);
	virtual int getId();
};

