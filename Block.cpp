#include "Block.h"



Block::Block(char id, TEXTURES textures, RendererType rendererType, float blockHeight)
{
	this->id = id;
	this->textures = textures;
	this->rendererType = rendererType;
	this->blockHeight = blockHeight;
}


Block::~Block()
{
}
