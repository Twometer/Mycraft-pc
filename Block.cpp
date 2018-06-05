#include "Block.h"



Block::Block(char id, TEXTURE topTex, TEXTURE sideTex, TEXTURE bottomTex, bool fluid)
{
	this->id = id;
	this->topTex = topTex;
	this->sideTex = sideTex;
	this->bottomTex = bottomTex;
	this->isFluid = fluid;
}


Block::~Block()
{
}
