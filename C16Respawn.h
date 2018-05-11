#pragma once
#include "IPacket.h"
class C16Respawn :
	public IPacket
{
public:
	C16Respawn();
	~C16Respawn();
	virtual void write(McBuffer* buffer);
	virtual void read(McBuffer* buffer);
	virtual int getId();
};

