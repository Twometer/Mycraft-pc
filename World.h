#pragma once
#include "concurrent_vector.h"
#include "Chunk.h"
using namespace std;
class World
{
public:
	Chunk** chunkArray;
	int chunkLen;
	unsigned char getBlock(int x, int y, int z);
	void setBlock(int x, int y, int z, unsigned char id);
	void render(bool transparencyPass);
	void addChunk(Chunk* chk);
	World();
	~World();
};

