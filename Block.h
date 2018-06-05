#pragma once
#include "Texture.h"
class Block
{
public:
	char id;
	bool isFluid;
	TEXTURE topTex;
	TEXTURE sideTex;
	TEXTURE bottomTex;

	Block(char id, TEXTURE topTex, TEXTURE sideTex, TEXTURE bottomTex, bool fluid);
	~Block();
};

