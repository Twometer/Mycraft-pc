#pragma once
#include "TextureHandler.h"
class PoweredRailsTextureHandler
	: public TextureHandler
{
private:
	TEXTURES off;
	TEXTURES on;
public:

	PoweredRailsTextureHandler(TEXTURES off, TEXTURES on) {
		this->off = off;
		this->on = on;
	}

	TEXTURES* getTextures(int meta) {
		return &(meta >= 8 ? on : off);
	}
};