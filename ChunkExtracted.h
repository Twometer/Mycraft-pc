#pragma once
class ChunkExtracted
{

public:
	char* storage;
	int storageLen;
	int bitmask;
	ChunkExtracted();
	~ChunkExtracted();
};

