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

	Block(char id, TEXTURE topTex, TEXTURE sideTex, TEXTURE bottomTex, RendererType rendererType);
	~Block();
};