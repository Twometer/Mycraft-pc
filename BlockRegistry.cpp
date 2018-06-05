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
	registerBlock(4, TEXTURE(3, 1)); // Cobblestone
	registerBlock(5, TEXTURE(29, 7)); // Wooden Planks
	registerBlock(7, TEXTURE(5, 0)); // Bedrock
	registerBlock(8, TEXTURE(2, 12), true); // Flowing water
	registerBlock(9, TEXTURE(3, 13), true); // Still water
	registerBlock(10, TEXTURE(0, 13), true); // Flowing Lava
	registerBlock(11, TEXTURE(0, 13), true); // Still lava
	registerBlock(12, TEXTURE(16, 9)); // Sand
	registerBlock(13, TEXTURE(9, 5)); // Gravel
	registerBlock(14, TEXTURE(4, 5)); // Gold Ore
	registerBlock(15, TEXTURE(4, 6)); // Iron Ore
	registerBlock(16, TEXTURE(1, 1)); // Coal Ore
	registerBlock(17, TEXTURE(29, 6), TEXTURE(28, 6), TEXTURE(29, 6)); // Log
	registerBlock(18, TEXTURE(13, 6)); // Leaves
	registerBlock(19, TEXTURE(0, 10)); // Sponge
	registerBlock(20, TEXTURE(0, 4)); // Glass
	registerBlock(21, TEXTURE(11, 6)); // Lapis Lazuli Ore
	registerBlock(22, TEXTURE(10, 6)); // Lapis Lazuli Block
	registerBlock(31, TEXTURE(15, 10)); // Tall grass
	registerBlock(32, TEXTURE(17, 1)); // Dead bush
	registerBlock(33, TEXTURE(23, 7), TEXTURE(22, 7), TEXTURE(20, 7)); // Piston
	registerBlock(35, TEXTURE(18, 11)); // Wool
	registerBlock(37, TEXTURE(18, 3)); // Dandelion
	registerBlock(38, TEXTURE(23, 3)); // Poppy
	registerBlock(39, TEXTURE(9, 7)); // Brown Mushroom
	registerBlock(40, TEXTURE(10, 7)); // Red Mushroom
	registerBlock(41, TEXTURE(3, 5)); // Gold Block
	registerBlock(42, TEXTURE(3, 6)); // Iron Block
	registerBlock(45, TEXTURE(15, 0)); // Bricks
	registerBlock(46, TEXTURE(18, 10), TEXTURE(17, 10), TEXTURE(16, 10)); // TNT
	registerBlock(47, TEXTURE(29, 7), TEXTURE(12, 0), TEXTURE(29, 7)); // Bookshelf
	registerBlock(48, TEXTURE(4, 1)); // Mossy Stone
	registerBlock(49, TEXTURE(19, 7)); // Obsidian
	registerBlock(50, TEXTURE(19, 10)); // Torch
	registerBlock(51, TEXTURE(15, 3)); // Fire
	registerBlock(52, TEXTURE(4, 7)); // Spawner
	registerBlock(56, TEXTURE(29, 1)); // Diamond Ore
	registerBlock(58, TEXTURE(13, 1), TEXTURE(11, 1), TEXTURE(29, 7)); // Crafting Table
	registerBlock(60, TEXTURE(11, 3), TEXTURE(30, 1), TEXTURE(30, 1)); // Farmland
	registerBlock(73, TEXTURE(4, 9)); // Redstone Ore
	registerBlock(74, TEXTURE(4, 9)); // Glowing Redstone Ore
	registerBlock(78, TEXTURE(30, 9)); // Snow
	registerBlock(79, TEXTURE(0, 6), true); // Ice
	registerBlock(80, TEXTURE(30, 9)); // Snow
	registerBlock(111, TEXTURE(25, 10)); // Lily pad

}

bool BlockRegistry::isTransparent(unsigned char id) {
	return id == 18 || id == 161 || id == 111 || isPlant(id) || id == 78;
}

bool BlockRegistry::isFluid(unsigned char id) {
	if (id <= 0 || id > 255) return registry[0];
	Block* block = *(registry + id);
	if (block == nullptr) return registry[0];
	return block->isFluid;
}

bool BlockRegistry::isPlant(unsigned char id) {
	return id == 31 || id == 32 || id == 37 || id == 38 || id == 39 || id == 40;
}

Block* BlockRegistry::getBlock(unsigned char id) {
	if (id <= 0 || id > 255) return registry[0];
	Block* block = *(registry + id);
	if (block == nullptr) return registry[0];
	return block;
}

void BlockRegistry::registerBlock(unsigned char id, TEXTURE topTex, TEXTURE sideTex, TEXTURE bottomTex, bool fluid) {
	registry[id] = new Block(id, topTex, sideTex, bottomTex, fluid);
}

void BlockRegistry::registerBlock(unsigned char id, TEXTURE tex, bool fluid) {
	registry[id] = new Block(id, tex, tex, tex, fluid);
}
void BlockRegistry::registerBlock(unsigned char id, TEXTURE topTex, TEXTURE sideTex, TEXTURE bottomTex) {
	registry[id] = new Block(id, topTex, sideTex, bottomTex, false);
}

void BlockRegistry::registerBlock(unsigned char id, TEXTURE tex) {
	registry[id] = new Block(id, tex, tex, tex, false);
}