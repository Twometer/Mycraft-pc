#include "Block.h"
#include "DefaultTextureHandler.h"


Block::Block(char id, RendererType rendererType, float blockHeight)
{
	this->id = id;
	this->rendererType = rendererType;
	this->blockHeight = blockHeight;
}

Block::Block(char id, TEXTURES textures, RendererType rendererType, float blockHeight)
{
	this->id = id;
	this->textureHandler = new DefaultTextureHandler(textures);
	this->rendererType = rendererType;
	this->blockHeight = blockHeight;
}


Block::~Block()
{
}
