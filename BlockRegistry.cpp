#include "BlockRegistry.h"
#include "StatefulTextureHandler.h"
#include "PoweredRailsTextureHandler.h"
#include "PoweredRailsVertexHandler.h"
#include "RailsTextureHandler.h"
#include "RailsVertexHandler.h"
#include "VinesVertexHandler.h"
#include "LaddersVertexHandler.h"
#include "CactusVertexHandler.h"
#include "LeavesTextureHandler.h"
#include "DoorTextureHandler.h"
#include "DoorVertexHandler.h"
#include "DoorBoundingBoxProvider.h"

Block** BlockRegistry::registry;

BlockRegistry::BlockRegistry()
= default;


BlockRegistry::~BlockRegistry()
= default;

void BlockRegistry::initialize() {
	registry = new Block*[255];
	for (int i = 0; i < 255; i++) *(registry + i) = nullptr;
	registerBlock(0, TEXTURE(23, 10)); // Missing texture block 
	registerBlock(1)
		->setTextureHandler((new StatefulTextureHandler(TEXTURE(2, 10)))
			->setState(1, TEXTURES(11, 10))
			->setState(2, TEXTURES(12, 10))
			->setState(3, TEXTURES(9, 10))
			->setState(4, TEXTURES(10, 10))
			->setState(5, TEXTURES(7, 10))
			->setState(6, TEXTURES(8, 10))
		); // Stone
	registerBlock(2, TEXTURE(8, 5), TEXTURE(5, 5), TEXTURE(30, 1)); // Grass
	registerBlock(3)
		->setTextureHandler((new StatefulTextureHandler(TEXTURE(30, 1)))
			->setState(1, TEXTURES(2, 1))
			->setState(2, TEXTURES(TEXTURE(0, 2), TEXTURE(31, 1), TEXTURE(30, 1)))
		); // Dirt
	registerBlock(4, TEXTURE(3, 1)); // Cobblestone
	registerBlock(5)
		->setTextureHandler((new StatefulTextureHandler(TEXTURE(29, 7)))
			->setState(1, TEXTURES(30, 7))
			->setState(2, TEXTURES(27, 7))
			->setState(3, TEXTURES(28, 7))
			->setState(4, TEXTURES(25, 7))
			->setState(5, TEXTURES(26, 7))
		); // Wooden Planks
	registerBlock(6, Plant)
		->setTextureHandler((new StatefulTextureHandler(TEXTURE(25, 9)))
			->setState(1, TEXTURES(27, 9))
			->setState(2, TEXTURES(23, 9))
			->setState(3, TEXTURES(24, 9))
			->setState(4, TEXTURES(22, 9))
			->setState(5, TEXTURES(26, 9))
		)->disableCollision(); // Sapling
	registerBlock(7, TEXTURE(5, 0)); // Bedrock
	registerBlock(8, TEXTURE(2, 12), Fluid)->disableCollision(); // Flowing water
	registerBlock(9, TEXTURE(3, 13), Fluid)->disableCollision(); // Still water
	registerBlock(10, TEXTURE(0, 13), Fluid)->disableCollision(); // Flowing Lava
	registerBlock(11, TEXTURE(0, 13), Fluid)->disableCollision(); // Still lava
	registerBlock(12)
		->setTextureHandler((new StatefulTextureHandler(TEXTURE(16, 9)))
			->setState(1, TEXTURES(7, 9))
		); // Sand
	registerBlock(13, TEXTURE(9, 5)); // Gravel
	registerBlock(14, TEXTURE(4, 5)); // Gold Ore
	registerBlock(15, TEXTURE(4, 6)); // Iron Ore
	registerBlock(16, TEXTURE(1, 1)); // Coal Ore
	registerBlock(17, TEXTURE(29, 6), TEXTURE(28, 6), TEXTURE(29, 6)); // Log
	registerBlock(18, Transparent)
		->setTextureHandler(new LeavesTextureHandler()); // Leaves
	registerBlock(19)
		->setTextureHandler((new StatefulTextureHandler(TEXTURE(0, 10)))
			->setState(1, TEXTURES(1, 10))
		); // Sponge
	registerBlock(20, TEXTURE(0, 4), Transparent); // Glass
	registerBlock(21, TEXTURE(11, 6)); // Lapis Lazuli Ore
	registerBlock(22, TEXTURE(10, 6)); // Lapis Lazuli Block
	registerBlock(23, TEXTURE(20, 7), TEXTURE(1, 2), TEXTURE(2, 10)); // Dispenser
	registerBlock(24, TEXTURE(21, 9), TEXTURE(19, 9), TEXTURE(17, 9)); // Sandstone
	registerBlock(25, TEXTURE(8, 6), TEXTURE(7, 6), TEXTURE(7, 6)); // Sandstone
	registerBlock(27, Flat, 0.1)->disableOcclusion()
		->disableCollision()
		->setTextureHandler(new PoweredRailsTextureHandler(
			TEXTURES(25, 8),
			TEXTURES(26, 8)
		))
		->setVertexHandler(new PoweredRailsVertexHandler()); // Golden Rail
	registerBlock(28, Flat, 0.1)->disableOcclusion()
		->disableCollision()
		->setTextureHandler(new PoweredRailsTextureHandler(
			TEXTURES(23, 8),
			TEXTURES(24, 8)
		))
		->setVertexHandler(new PoweredRailsVertexHandler()); // Detector rail
	registerBlock(30, TEXTURE(27, 10), Plant)->disableCollision(); // Cobweb
	registerBlock(31, TEXTURE(15, 10), Plant)->disableCollision()
		->disableCollision(); // Tall grass
	registerBlock(32, TEXTURE(17, 1), Plant)->disableCollision(); // Dead bush
	registerBlock(33, TEXTURE(23, 7), TEXTURE(22, 7), TEXTURE(20, 7)); // Piston
	registerBlock(35, TEXTURE(18, 11)); // Wool
	registerBlock(37, TEXTURE(18, 3), Plant)->disableCollision(); // Dandelion
	registerBlock(38, TEXTURE(23, 3), Plant)->disableCollision(); // Poppy
	registerBlock(39, TEXTURE(9, 7), Plant)->disableCollision(); // Brown Mushroom
	registerBlock(40, TEXTURE(10, 7), Plant)->disableCollision(); // Red Mushroom
	registerBlock(41, TEXTURE(3, 5)); // Gold Block
	registerBlock(42, TEXTURE(3, 6)); // Iron Block

	registerBlock(44, Transparent, 0.5f)
		->setSpecialBlock(Slab)
		->setTextureHandler((new StatefulTextureHandler(TEXTURE(14, 10), TEXTURE(13, 10), TEXTURE(14, 10)))
			->setState(1, TEXTURES(TEXTURE(21, 9), TEXTURE(19, 9), TEXTURE(17, 9)))
			->setState(2, TEXTURES(29, 7))
			->setState(3, TEXTURES(3, 1))
			->setState(4, TEXTURES(15, 0))
			->setState(5, TEXTURES(3, 10))
			->setState(6, TEXTURES(14, 7))
			->setState(7, TEXTURES(19, 8))
		); // Stone slab

	registerBlock(45, TEXTURE(15, 0)); // Bricks
	registerBlock(46, TEXTURE(18, 10), TEXTURE(17, 10), TEXTURE(16, 10)); // TNT
	registerBlock(47, TEXTURE(29, 7), TEXTURE(12, 0), TEXTURE(29, 7)); // Bookshelf
	registerBlock(48, TEXTURE(4, 1)); // Mossy Stone
	registerBlock(49, TEXTURE(19, 7)); // Obsidian
	registerBlock(50, TEXTURE(19, 10), Plant)->disableCollision(); // Torch
	registerBlock(51, TEXTURE(15, 3), Plant)->disableCollision(); // Fire
	registerBlock(52, TEXTURE(4, 7)); // Spawner
	registerBlock(56, TEXTURE(29, 1)); // Diamond Ore
	registerBlock(57, TEXTURE(28, 1)); // Diamond Block
	registerBlock(58, TEXTURE(13, 1), TEXTURE(11, 1), TEXTURE(29, 7)); // Crafting Table
	registerBlock(59, TEXTURE(3, 11), Plant)->disableCollision(); // Wheat
	registerBlock(60, TEXTURE(11, 3), TEXTURE(30, 1), TEXTURE(30, 1), Transparent, 0.95); // Farmland
	registerBlock(61, TEXTURE(31, 3), TEXTURE(28, 3), TEXTURE(31, 3)); // Furnace
	registerBlock(62, TEXTURE(31, 3), TEXTURE(29, 3), TEXTURE(31, 3)); // Burning furnace
	registerBlock(64, Transparent)
		->disableOcclusion()
		->setTextureHandler(new DoorTextureHandler(TEXTURES(16, 2), TEXTURES(15, 2)))
		->setVertexHandler(new DoorVertexHandler())
		->setBoundingBoxProvider(new DoorBoundingBoxProvider()); // Wooden door
	registerBlock(65, TEXTURE(9, 6), FlatSide)
		->disableCollision()
		->disableOcclusion()
		->setVertexHandler(new LaddersVertexHandler()); // Ladders
	registerBlock(66, Flat, 0.1)
		->disableOcclusion()
		->disableCollision()
		->setTextureHandler(new RailsTextureHandler(TEXTURES(27, 8), TEXTURES(28, 8)))
		->setVertexHandler(new RailsVertexHandler());
	registerBlock(71, Transparent)
		->disableOcclusion()
		->setTextureHandler(new DoorTextureHandler(TEXTURES(10, 2), TEXTURES(9, 2)))
		->setVertexHandler(new DoorVertexHandler())
		->setBoundingBoxProvider(new DoorBoundingBoxProvider()); // Iron door
	registerBlock(73, TEXTURE(4, 9)); // Redstone Ore
	registerBlock(74, TEXTURE(4, 9)); // Glowing Redstone Ore
	registerBlock(75, TEXTURE(5, 9), Plant)->disableCollision(); // Redstone torch off
	registerBlock(76, TEXTURE(6, 9), Plant)->disableCollision(); // Redstone torch on
	registerBlock(78, TEXTURE(30, 9), Transparent, 0.1f)->disableOcclusion(); // Snow
	registerBlock(79, TEXTURE(0, 6), Fluid); // Ice
	registerBlock(80, TEXTURE(30, 9)); // Snow
	registerBlock(81, TEXTURE(18, 0), TEXTURE(17, 0), TEXTURE(16, 0), Transparent)
		->setVertexHandler(new CactusVertexHandler()); // Cactus
	registerBlock(87, TEXTURE(13, 7)); // Netherrack
	registerBlock(89, TEXTURE(2, 5)); // Glowstone
	registerBlock(90, TEXTURE(31, 7)); // Nether Portal
	registerBlock(91, TEXTURE(12, 8), TEXTURE(8, 8), TEXTURE(12, 8)); // Jack'o'lantern
	registerBlock(97, TEXTURE(2, 10)); // Stone Monster egg
	registerBlock(98)
		->setTextureHandler((new StatefulTextureHandler(TEXTURES(3, 10)))
			->setState(1, TEXTURES(6, 10))
			->setState(2, TEXTURES(5, 10))
			->setState(3, TEXTURES(4, 10))
		); // Stone Bricks
	registerBlock(103, TEXTURE(3, 7), TEXTURE(0, 7), TEXTURE(3, 7)); // Melon
	registerBlock(106, TEXTURE(24, 10), FlatSide)
		->disableCollision()
		->disableOcclusion()
		->setVertexHandler(new VinesVertexHandler()); // Vines
	registerBlock(110, TEXTURE(12, 7), TEXTURE(11, 7), TEXTURE(30, 1)); // Mycelium
	registerBlock(111, TEXTURE(25, 10), Flat); // Lily pad
	registerBlock(112, TEXTURE(14, 7)); // Nether bricks
	registerBlock(115, TEXTURE(17, 7), Plant)->disableCollision(); // Nether wart
	registerBlock(121, TEXTURE(10, 3)); // Endstone
	registerBlock(123, TEXTURE(2, 9)); // Redstone Lamp (off)
	registerBlock(124, TEXTURE(3, 9)); // Redstone Lamp (on)
	registerBlock(126, TEXTURE(29, 7), Transparent, 0.5f)
		->setSpecialBlock(Slab)
		->setTextureHandler((new StatefulTextureHandler(TEXTURES(29, 7)))
			->setState(1, TEXTURES(30, 7))
			->setState(2, TEXTURES(27, 7))
			->setState(3, TEXTURES(28, 7))
			->setState(4, TEXTURES(25, 7))
			->setState(5, TEXTURES(26, 7))
		); // Wood slab
	registerBlock(129, TEXTURE(3, 3)); // Emerald Ore
	registerBlock(133, TEXTURE(2, 3)); // Emerald Block
	registerBlock(137, TEXTURE(8, 1)); // Command Block
	registerBlock(141, TEXTURE(26, 0), Plant)->disableCollision(); // Carrots
	registerBlock(142, TEXTURE(3, 8), Plant)->disableCollision(); // Potatoes
	registerBlock(152, TEXTURE(29, 8)); // Redstone block
	registerBlock(153, TEXTURE(20, 8)); // Quartz Ore 
	registerBlock(155, TEXTURE(19, 8)); // Quartz Block
	registerBlock(157, Flat, 0.1)->disableOcclusion()
		->disableCollision()
		->setTextureHandler(new PoweredRailsTextureHandler(
			TEXTURES(21, 8),
			TEXTURES(22, 8)
		))
		->setVertexHandler(new PoweredRailsVertexHandler()); // Activator rail
	registerBlock(159, TEXTURE(25, 5)); // Hardened Clay
	registerBlock(161, TEXTURE(18, 6), Transparent); // Leaves 2
	registerBlock(162, TEXTURE(21, 6), TEXTURE(20, 6), TEXTURE(21, 6), Transparent); // Acacia leaves
	registerBlock(165, TEXTURE(29, 9), Transparent); // Slime block
	registerBlock(166, TEXTURE(20, 11), Transparent); // Barrier
	registerBlock(168, TEXTURE(6, 8)); // Prismarine
	registerBlock(169, TEXTURE(28, 9)); // Prismarine Lantern
	registerBlock(170, TEXTURE(28, 5), TEXTURE(27, 5), TEXTURE(28, 5)); // Hay bale
	registerBlock(172, TEXTURE(10, 5)); // Hay bale
}

Block* BlockRegistry::getBlock(unsigned char id) {
	if (id == 0) return nullptr;
	Block* block = *(registry + id);
	if (block == nullptr) return registry[0];
	return block;
}

Block* BlockRegistry::registerBlock(unsigned char id, TEXTURE topTex, TEXTURE sideTex, TEXTURE bottomTex, RendererType type, float blockHeight) {
	Block* b = new Block(id, TEXTURES(topTex, sideTex, bottomTex), type, blockHeight);
	registry[id] = b;
	return b;
}

Block* BlockRegistry::registerBlock(unsigned char id, TEXTURE tex, RendererType type, float blockHeight) {
	Block* b = new Block(id, TEXTURES(tex), type, blockHeight);
	registry[id] = b;
	return b;
}
Block * BlockRegistry::registerBlock(unsigned char id, RendererType rendererType, float blockHeight)
{
	Block* b = new Block(id, rendererType, blockHeight);
	registry[id] = b;
	return b;
}
Block* BlockRegistry::registerBlock(unsigned char id, TEXTURE topTex, TEXTURE sideTex, TEXTURE bottomTex, float blockHeight) {
	Block* b = new Block(id, TEXTURES(topTex, sideTex, bottomTex), Solid, blockHeight);
	registry[id] = b;
	return b;
}

Block* BlockRegistry::registerBlock(unsigned char id, TEXTURE tex, float blockHeight) {
	Block* b = new Block(id, TEXTURES(tex), Solid, blockHeight);
	registry[id] = b;
	return b;
}