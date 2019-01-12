#pragma once
#include <unordered_map>
#include "Block.h"
#include "Texture.h"
using namespace std;
class BlockRegistry
{
private:
	static Block** registry;
	static Block* registerBlock(unsigned char id, RendererType rendererType = Solid, float blockHeight = 1.0f);
	static Block* registerBlock(unsigned char id, TEXTURE topTex, TEXTURE sideTex, TEXTURE bottomTex, float blockHeight = 1.0f);
	static Block* registerBlock(unsigned char id, TEXTURE allTex, float blockHeight = 1.0f);
	static Block* registerBlock(unsigned char id, TEXTURE topTex, TEXTURE sideTex, TEXTURE bottomTex, RendererType type, float blockHeight = 1.0f);
	static Block* registerBlock(unsigned char id, TEXTURE allTex, RendererType type, float blockHeight = 1.0f);
public:
	BlockRegistry();
	~BlockRegistry();
	static void initialize();
	static Block* getBlock(unsigned char id);
};

