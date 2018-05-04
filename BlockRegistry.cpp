#include "BlockRegistry.h"

Block** BlockRegistry::registry;

BlockRegistry::BlockRegistry()
{
}


BlockRegistry::~BlockRegistry()
{
}

void BlockRegistry::initialize() {
	registry = new Block*[255];
	for (int i = 0; i < 255; i++) *(registry + i) = nullptr;
	registerBlock(0, TEXTURE(23, 10)); // Missing texture block 
	registerBlock(1, TEXTURE(2, 10)); // Stone
	registerBlock(2, TEXTURE(8, 5), TEXTURE(5, 5), TEXTURE(30, 1)); // Grass
	registerBlock(3, TEXTURE(30, 1)); // Dirt
	registerBlock(7, TEXTURE(5, 0)); // Bedrock
	registerBlock(8, TEXTURE(2, 12)); // Flowing water
	registerBlock(9, TEXTURE(3, 13)); // Still water
	registerBlock(12, TEXTURE(16, 9)); // Sand
	registerBlock(13, TEXTURE(9, 5)); // Gravel
	registerBlock(14, TEXTURE(4, 5)); // Gold Ore
	registerBlock(15, TEXTURE(4, 6)); // Iron Ore
	registerBlock(16, TEXTURE(1, 1)); // Coal Ore
	registerBlock(17, TEXTURE(28, 6)); // Log
	registerBlock(18, TEXTURE(13, 6)); // Leaves
	registerBlock(21, TEXTURE(11, 6)); // Lapis Lazuli Ore
	registerBlock(56, TEXTURE(29, 1)); // Diamond Ore
	registerBlock(78, TEXTURE(30, 9)); // Snow
	registerBlock(80, TEXTURE(30, 9)); // Snow

}

Block* BlockRegistry::getBlock(unsigned char id) {
	if (id <= 0 || id > 255) return registry[0];
	Block* block = *(registry + id);
	if (block == nullptr) return registry[0];
	return block;
}

void BlockRegistry::registerBlock(unsigned char id, TEXTURE topTex, TEXTURE sideTex, TEXTURE bottomTex) {
	registry[id] = new Block(id, topTex, sideTex, bottomTex);
}

void BlockRegistry::registerBlock(unsigned char id, TEXTURE tex) {
	registry[id] = new Block(id, tex, tex, tex);
}