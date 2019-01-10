#pragma once
#include <unordered_map>
#include "Block.h"
#include "Texture.h"
using namespace std;
class BlockRegistry
{
private:
	static Block** registry;
	static void registerBlock(unsigned char id, TEXTURE topTex, TEXTURE sideTex, TEXTURE bottomTex);
	static void registerBlock(unsigned char id, TEXTURE allTex);
	static void registerBlock(unsigned char id, TEXTURE topTex, TEXTURE sideTex, TEXTURE bottomTex, RendererType type);
	static void registerBlock(unsigned char id, TEXTURE allTex, RendererType type);
public:
	BlockRegistry();
	~BlockRegistry();
	static void initialize();
	static bool isFluid(unsigned char id);
	static bool isTransparent(unsigned char id);
	static bool isPlant(unsigned char id);
	static bool isSlab(unsigned char id);
	static Block* getBlock(unsigned char id);
};

