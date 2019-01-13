#pragma once
#include "TextureHandler.h"
#include "DoorState.h"

class DoorTextureHandler
	: public TextureHandler
{
private:
	TEXTURES upper;
	TEXTURES lower;

public:

	DoorTextureHandler(TEXTURES upper, TEXTURES lower){
		this->upper = upper;
		this->lower = lower;
	}

	TEXTURES* getTextures(int meta) {
		DOORSTATE doorstate(meta);
		if (doorstate.half == UPPER)
			return &upper;
		else return &lower;
	}

};