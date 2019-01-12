#pragma once
#include <vector>
#include "GLFW/glfw3.h"
#include "Section.h"
#include "Mesh.h"
#include "VertexHandler.h"
#include "Block.h"
class Block;

class SectionBuilder
{
private:
	Section* chunk;
	
	int absX;
	int absY;
	int absZ;
	int x;
	int y;
	int z;
	int xo, yo, zo;
	unsigned char getBlock(Block* block, int x, int y, int z);
public:
	Mesh* regularMesh;
	Mesh* transparentMesh;

	void drawDisplacedVertices(const GLfloat* textures, const GLfloat* vertices, int x, int y, int z, int tX, int tY, GLfloat col, Mesh* mesh, int face, float height, float yOffset, int meta, VertexHandler* vertexHandler, RendererType r);
	void build(int xoff, int yoff, int zoff);
	SectionBuilder(Section* chunk);
	~SectionBuilder();
};

