#pragma once
#include "Texture.h"

enum RendererType {
	Fluid,
	Transparent,
	Plant,
	Solid
};

class Block
{
public:
	char id;
	RendererType rendererType;
	TEXTURE topTex;
	TEXTURE sideTex;
	TEXTURE bottomTex;
	float blockHeight;
	bool canOcclude = true;

	Block(char id, TEXTURE topTex, TEXTURE sideTex, TEXTURE bottomTex, RendererType rendererType, float blockHeight);
	~Block();

	bool isTransparent() const
	{
		return rendererType == Plant || rendererType == Transparent;
	}

	Block* disableOcclusion()
	{
		canOcclude = false;
		return this;
	}
};