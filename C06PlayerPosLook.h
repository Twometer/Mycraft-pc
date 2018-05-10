#pragma once
#include "IPacket.h"
class C06PlayerPosLook :
	public IPacket
{
private:
	double x;
	double y;
	double z;
	float yaw;
	float pitch;
	bool onGround;
public:
	C06PlayerPosLook(double x, double y, double z, float yaw, float pitch, bool onGround);
	~C06PlayerPosLook();
	virtual void write(McBuffer* buffer);
	virtual void read(McBuffer* buffer);
	virtual int getId();
};
