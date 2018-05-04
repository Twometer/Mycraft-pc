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
	registerBlock(0, 23, 10); // Missing texture block 
	registerBlock(1, 2, 10); // Stone
	registerBlock(2, 5, 5); // Grass
	registerBlock(3, 30, 1); // Dirt
	registerBlock(7, 5, 0); // Bedrock
	registerBlock(8, 2, 12); // Flowing water
	registerBlock(9, 3, 13); // Still water
	registerBlock(17, 28, 6); // Log
	registerBlock(18, 13, 6); // Leaves
}

Block* BlockRegistry::getBlock(unsigned char id) {
	if (id <= 0 || id > 255) return registry[0];
	Block* block = *(registry + id);
	if (block == nullptr) return registry[0];
	return block;
}

void BlockRegistry::registerBlock(unsigned char id, int texX, int texY) {
	registry[id] = new Block(id, texX, texY);
}