#pragma once
#include "TextureHandler.h"
class DefaultTextureHandler :
	public TextureHandler
{
public:
	TEXTURES textures;

	DefaultTextureHandler(TEXTURES textures) {
		this->textures = textures;
	}

	~DefaultTextureHandler() = default;

	TEXTURES* getTextures(int meta) {
		return &textures;
	}
};

