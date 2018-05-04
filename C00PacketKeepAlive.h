#pragma once
#include "IPacket.h"
class C00PacketKeepAlive :
	public IPacket
{
private:
	int id;
public:
	C00PacketKeepAlive(int id);
	~C00PacketKeepAlive();
	virtual void write(McBuffer* buffer);
	virtual void read(McBuffer* buffer);
	virtual int getId();
};

