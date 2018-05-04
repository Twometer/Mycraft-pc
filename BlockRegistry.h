#pragma once
#include <unordered_map>
#include "Block.h"
using namespace std;
class BlockRegistry
{
private:
	static Block** registry;
	static void registerBlock(unsigned char id, int texX, int texY);
public:
	BlockRegistry();
	~BlockRegistry();
	static void initialize();
	static Block* getBlock(unsigned char id);
};

