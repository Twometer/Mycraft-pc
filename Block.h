#pragma once
#include "TextureHandler.h"
#include "VertexHandler.h"
#include "BoundingBoxProvider.h"

enum RendererType {
	Fluid,
	Transparent,
	Plant,
	Solid,
	Flat,
	FlatSide
};

enum SpecialBlock {
	None,
	Slab
};

class Block
{
public:
	unsigned char id;

	RendererType rendererType;
	SpecialBlock specialBlock = None;

	TextureHandler* textureHandler;
	VertexHandler* vertexHandler;
	BoundingBoxProvider* bbProvider;

	float blockHeight;
	float yOffset;
	bool canOcclude = true;
	bool canCollide = true;

	Block(char id, RendererType rendererType, float blockHeight);
	Block(char id, TEXTURES textures, RendererType rendererType, float blockHeight);
	~Block();

	TEXTURES* getTextures(int meta) {
		return textureHandler->getTextures(meta);
	}

	Block* setTextureHandler(TextureHandler* handler) {
		this->textureHandler = handler;
		return this;
	}

	Block* setVertexHandler(VertexHandler* handler) {
		vertexHandler = handler;
		return this;
	}

	Block* setBoundingBoxProvider(BoundingBoxProvider* provider) {
		this->bbProvider = provider;
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
		return rendererType == Plant || rendererType == Transparent || rendererType == Flat || rendererType == FlatSide;
	}

	Block* disableCollision()
	{
		canCollide = false;
		return this;
	}

	Block* disableOcclusion()
	{
		canOcclude = false;
		return this;
	}
};