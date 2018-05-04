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
public:
	BlockRegistry();
	~BlockRegistry();
	static void initialize();
	static Block* getBlock(unsigned char id);
};

