#pragma once
#include "Texture.h"
#include "Textures.h"
#include <map>

enum RendererType {
	Fluid,
	Transparent,
	Plant,
	Solid,
	Flat
};

enum SpecialBlock {
	None,
	Slab
};

class Block
{
public:
	char id;
	RendererType rendererType;
	SpecialBlock specialBlock;

	TEXTURES textures;
	std::map<int, TEXTURES> stateMap;
	bool useStateMap = false;

	float blockHeight;
	float yOffset;
	bool canOcclude = true;

	Block(char id, TEXTURES textures, RendererType rendererType, float blockHeight);
	~Block();

	TEXTURES getTextures(int meta) {
		return useStateMap ? stateMap[meta] : textures;
	}

	Block* enableStateMap() {
		useStateMap = true;
		setState(0, textures);
		return this;
	}

	Block* setState(int meta, TEXTURES textures) {
		stateMap.insert(std::pair<int, TEXTURES>(meta, textures));
		return this;
	}

	Block* setSpecialBlock(SpecialBlock specialBlock) {
		this->specialBlock = specialBlock;
		return this;
	}

	float getYOffset(int meta) {
		return specialBlock == Slab && meta > 7 ? 0.5f : 0.0f;
	}

	bool isTransparent() const
	{
		return rendererType == Plant || rendererType == Transparent || rendererType == Flat;
	}

	Block* disableOcclusion()
	{
		canOcclude = false;
		return this;
	}
};