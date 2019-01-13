#pragma once
#include "IPacket.h"
#include "Position.h"
class C08PlayerBlockPlacement :
	public IPacket
{
private:
	POSITION position;
	char face;
	char cx;
	char cy;
	char cz;
public:
	C08PlayerBlockPlacement(POSITION position, char face, char cx, char cy, char cz);
	~C08PlayerBlockPlacement();
	virtual void write(McBuffer* buffer);
	virtual void read(McBuffer* buffer);
	virtual int getId();
};

