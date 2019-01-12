#pragma once
#include "Textures.h"

class TextureHandler
{
public:
	TextureHandler();
	~TextureHandler();
	virtual TEXTURES* getTextures(int meta) = 0;
};

