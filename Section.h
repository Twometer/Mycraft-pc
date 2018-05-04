#pragma once
#include "GLFW\glfw3.h"
#include "Chunk.h"
class Chunk;
class VboBuilder;
class Section
{
private:
	Chunk* parent;
	unsigned char* data;
	int x;
	int idx;
	int z;
	int dataLen;
	int vertexCount;
	VboBuilder* builder;
	GLuint vertexBuffer;
	GLuint colorBuffer;
	GLuint textureBuffer;
	int state = 0;
public:
	enum State {
		STATE_SHOULD_BUILD = 0,
		STATE_AWAITING_BUILD = 1,
		STATE_SHOULD_UPLOAD = 2,
		STATE_SHOULD_RENDER = 3
	};

	void setBlock(int x, int y, int z, unsigned char id);
	void generate();
	unsigned char getBlock(int x, int y, int z);
	unsigned char* getBlockPointer(int x, int y, int z);
	void buildVertexData();
	void uploadVertexData();
	void render(bool inFrustum);
	static void resetData();
	Section(Chunk* parent, int x, int idx, int z);
	~Section();
};