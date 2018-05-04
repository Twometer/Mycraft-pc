#include "Block.h"



Block::Block(char id, TEXTURE topTex, TEXTURE sideTex, TEXTURE bottomTex)
{
	this->id = id;
	this->topTex = topTex;
	this->sideTex = sideTex;
	this->bottomTex = bottomTex;
}


Block::~Block()
{
}
