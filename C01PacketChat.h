#pragma once
#include "IPacket.h"
class C01PacketChat :
	public IPacket
{
private:
	const char* message;
public:
	C01PacketChat(const char* message);
	~C01PacketChat();
	virtual void write(McBuffer* buffer);
	virtual void read(McBuffer* buffer);
	virtual int getId();
};

