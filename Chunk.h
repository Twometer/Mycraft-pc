#pragma once
#include <vector>
#include "Section.h"
#include "ChunkExtracted.h"
class Section;
class Chunk
{
private:
	Section * * sections;
	bool deleted;
public:
	int x;
	int z;
	unsigned char getBlock(int x, int y, int z);
	void setBlock(int x, int y, int z, unsigned char id, bool updateSection);
	void buildVertexData();
	void uploadVertexData();
	void render(bool transparencyPass, bool ignoreFrustum);
	void check_deletion(bool transparencyPass);
	void destroy();
	void destructionComplete(int idx);
	void initialize(ChunkExtracted* data);
	void reload();
	Chunk(int x, int z);
	~Chunk();
};

