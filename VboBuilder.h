#pragma once
#include <vector>
#include "GLFW/glfw3.h"
#include "Section.h"
class VboBuilder
{
private:
	Section* chunk;
	void drawDisplacedVertices(const GLfloat* textures, const GLfloat* vertices, int x, int y, int z, int tX, int tY);
	int absX;
	int absY;
	int absZ;
	int x;
	int y;
	int z;
	int xo, yo, zo;
	unsigned char getBlock(int x, int y, int z);
public:
	GLfloat * vertices;
	int verticesAlloc;
	
	GLfloat * colors;
	int colorsAlloc;

	GLfloat * textureCoords;
	int textureCoordsAlloc;

	void build(int xoff, int yoff, int zoff);
	VboBuilder(Section* chunk);
	~VboBuilder();
};

