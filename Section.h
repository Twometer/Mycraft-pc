#pragma once
#include "GLFW\glfw3.h"
#include "Chunk.h"
class Chunk;
class SectionBuilder;
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
	int vertexCountX;
	SectionBuilder* builder;
	GLuint vertexBuffer;
	GLuint vertexBufferX;
	GLuint colorBuffer;
	GLuint colorBufferX;
	GLuint textureBuffer;
	GLuint textureBufferX;
	int state = 0;
	bool continueRender;
	bool dataCleared;
public:
	enum State {
		STATE_SHOULD_BUILD = 0,
		STATE_AWAITING_BUILD = 1,
		STATE_SHOULD_UPLOAD = 2,
		STATE_SHOULD_RENDER = 3,
		STATE_SHOULD_DELETE = 4,
		STATE_DELETED = 5
	};

	void setBlock(int x, int y, int z, unsigned char id, bool update);
	void generate();
	unsigned char getBlock(int x, int y, int z);
	unsigned char* getBlockPointer(int x, int y, int z);
	void buildVertexData();
	void uploadVertexData();
	void destroy();
	void check_deletion(bool transparencyPass);
	void render(bool transparencyPass, bool inFrustum);
	void reload();
	static void resetData();
	Section(Chunk* parent, int x, int idx, int z);
	~Section();
};