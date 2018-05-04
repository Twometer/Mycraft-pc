#pragma once
#include "Texture.h"
class Block
{
public:
	char id;
	TEXTURE topTex;
	TEXTURE sideTex;
	TEXTURE bottomTex;

	Block(char id, TEXTURE topTex, TEXTURE sideTex, TEXTURE bottomTex);
	~Block();
};

