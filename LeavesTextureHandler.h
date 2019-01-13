#pragma once
#include "TextureHandler.h"

class LeavesTextureHandler
	: public TextureHandler
{
private:
	TEXTURES textures[4] = {
		TEXTURES(13, 6),
		TEXTURES(18, 6),
		TEXTURES(15, 6),
		TEXTURES(16, 6)
	};

public:
	TEXTURES* getTextures(int meta) {
		meta &= 0b00000011;
		return &(textures[meta]);
	}

};