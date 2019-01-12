#pragma once
#include "TextureHandler.h"
class RailsTextureHandler
	: public TextureHandler
{
private:
	TEXTURES curved;
	TEXTURES straight;
public:

	RailsTextureHandler(TEXTURES straight, TEXTURES curved) {
		this->curved = curved;
		this->straight = straight;
	}

	TEXTURES* getTextures(int meta) {
		return &(meta >= 6 && meta <= 9 ? curved : straight);
	}
};