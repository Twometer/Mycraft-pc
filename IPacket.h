#pragma once
#include "McBuffer.h"
class IPacket
{
public:
	IPacket();
	~IPacket();
	virtual void write(McBuffer* buffer) = 0;
	virtual void read(McBuffer* buffer) = 0;
	virtual int getId() = 0;
};

