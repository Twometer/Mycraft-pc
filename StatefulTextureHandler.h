#pragma once
#include "TextureHandler.h"
#include <map>
class StatefulTextureHandler
	: public TextureHandler
{
private:
	std::map<int, TEXTURES> stateMap;

public:

	StatefulTextureHandler(TEXTURE top, TEXTURE side, TEXTURE bottom) {
		setState(0, TEXTURES(top, side, bottom));
	}

	StatefulTextureHandler(TEXTURES baseTexture) {
		setState(0, baseTexture);
	}

	StatefulTextureHandler* setState(int state, TEXTURES textures) {
		stateMap.insert(std::pair<int, TEXTURES>(state, textures));
		return this;
	}

	TEXTURES* getTextures(int meta) {
		return &stateMap[meta];
	}

};