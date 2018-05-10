#pragma once
#include "IPacket.h"
#include "Position.h"
enum ACTION {
	START_DESTROY_BLOCK,
	ABORT_DESTROY_BLOCK,
	STOP_DESTROY_BLOCK,
	DROP_ALL_ITEMS,
	DROP_ITEM,
	RELEASE_USE_ITEM
};
class C07PlayerDigging :
	public IPacket
{
private:
	ACTION status;
	POSITION position;
	char face;
public:
	C07PlayerDigging(ACTION status, POSITION pos, char face);
	~C07PlayerDigging();
	virtual void write(McBuffer* buffer);
	virtual void read(McBuffer* buffer);
	virtual int getId();
};

