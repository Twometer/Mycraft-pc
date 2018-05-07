#include "SectionBuilder.h"
#include <vector>
#include "GLFW/glfw3.h"
#include <stdio.h>
#include "OpenGLRenderer.h"
#include "BlockRegistry.h"

using namespace std;

static const GLfloat vertices_negative_x[] = {
	0.0f, 0.0f, 0.0f, // Neg x
	0.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 1.0f,
	0.0f, 0.0f, 0.0f, // Neg x
	0.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 0.0f
};

static const GLfloat tvertices_negative_x[] = {
	1.0f, 1.0f,
	0.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 0.0f
};

static const GLfloat vertices_negative_y[] = {
	1.0f, 0.0f, 1.0f, // Neg y
	0.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 1.0f, // Neg y
	0.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 0.0f,
};

static const GLfloat tvertices_negative_y[] = {
	1.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,
	0.0f, 0.0f
};

static const GLfloat vertices_negative_z[] = {
	1.0f, 1.0f, 0.0f, // Neg z
	0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,  // Neg z
	1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
};

static const GLfloat tvertices_negative_z[] = {
	1.0f, 0.0f,
	0.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f
};

static const GLfloat vertices_positive_x[] = {
	1.0f, 0.0f, 0.0f, // Pos x
	1.0f, 1.0f, 1.0f,
	1.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 1.0f, // Pos x
	1.0f, 0.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
};

static const GLfloat tvertices_positive_x[] = {
	0.0f, 1.0f,
	1.0f, 0.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 1.0f,
	0.0f, 0.0f,
};

static const GLfloat vertices_positive_y[] = {
	1.0f, 1.0f, 1.0f, // Pos y
	1.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 1.0f, // Posy
	0.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 1.0f,
};

static const GLfloat tvertices_positive_y[] = {
	1.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 0.0f,
	0.0f, 1.0f
};


static const GLfloat vertices_positive_z[] = {
	0.0f, 1.0f, 1.0f, // Pos z
	0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 1.0f, // Pos z
	0.0f, 1.0f, 1.0f,
	1.0f, 0.0f, 1.0f
};

static const GLfloat tvertices_positive_z[] = {
	1.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 0.0f,
	0.0f, 1.0f
};


SectionBuilder::SectionBuilder(Section* chunk)
{
	this->chunk = chunk;
	this->vertices = new GLfloat[100000]; // Three entries per vertex
	this->colors = new GLfloat[100000]; // Three entries per vertex
	this->textureCoords = new GLfloat[75000]; // Two entries per vertex

	this->verticesX = new GLfloat[25000]; // 3
	this->colorsX = new GLfloat[25000]; // 3
	this->textureCoordsX = new GLfloat[19000]; // 2
}

SectionBuilder::~SectionBuilder()
{
	delete[] vertices;
	delete[] colors;
	delete[] textureCoords;

	delete[] verticesX;
	delete[] colorsX;
	delete[] textureCoordsX;
}

void SectionBuilder::drawDisplacedVertices(const GLfloat* textures, const GLfloat* vertices, int x, int y, int z, int texX, int texY, GLfloat col, GLfloat* vertexPtr, GLfloat* texPtr, GLfloat* colorPtr, int* vertexC, int* texC, int * colorC) {

	for (int i = 0; i < 18; i += 3) {
		*(vertexPtr + ((*vertexC)++)) = *(vertices + i) + x;
		*(vertexPtr + ((*vertexC)++)) = *(vertices + i + 1) + y;
		*(vertexPtr + ((*vertexC)++)) = *(vertices + i + 2) + z;
		*(colorPtr + ((*colorC)++)) = col;
		*(colorPtr + ((*colorC)++)) = col;
		*(colorPtr + ((*colorC)++)) = col;
	}

	GLfloat d = 0.03125;

	for (int i = 0; i < 12; i += 2) {
		*(texPtr + ((*texC)++)) = (*(textures + i)) * d + texX * d;
		*(texPtr + ((*texC)++)) = (*(textures + i + 1)) * d + texY * d;
	}
}

void SectionBuilder::build(int xo, int yo, int zo) {
	Section chk = *chunk;
	this->zo = zo;
	this->yo = yo;
	this->xo = xo;
	for (x = 0; x < 16; x++) {
		absX = x + xo;
		for (y = 0; y < 16; y++) {
			absY = y + yo;
			for (z = 0; z < 16; z++) {
				absZ = z + zo;
				unsigned char blockId = chk.getBlock(x, y, z);
				bool isTransparent = BlockRegistry::isFluid(blockId);
				GLfloat* vertexPtr = isTransparent ? verticesX : vertices;
				GLfloat* texPtr = isTransparent ? textureCoordsX : textureCoords;
				GLfloat* colorPtr = isTransparent ? colorsX : colors;
				
				int* vertexC = isTransparent ? &verticesAllocX : &verticesAlloc;
				int* colorC = isTransparent ? &colorsAllocX : &colorsAlloc;
				int* texC = isTransparent ? &textureCoordsAllocX : &textureCoordsAlloc;

				if (blockId != 0) {
					Block* block = BlockRegistry::getBlock(blockId);
					
					int bx = block->sideTex.x;
					int by = block->sideTex.y;
					if (getBlock(blockId, x + 1, y, z) == 0) drawDisplacedVertices(tvertices_positive_x, vertices_positive_x, absX, absY, absZ, bx, by, 0.75, vertexPtr, texPtr, colorPtr, vertexC, texC, colorC);
					if (getBlock(blockId, x - 1, y, z) == 0) drawDisplacedVertices(tvertices_negative_x, vertices_negative_x, absX, absY, absZ, bx, by, 0.75, vertexPtr, texPtr, colorPtr, vertexC, texC, colorC);

					if (getBlock(blockId, x, y, z + 1) == 0) drawDisplacedVertices(tvertices_positive_z, vertices_positive_z, absX, absY, absZ, bx, by, 0.65, vertexPtr, texPtr, colorPtr, vertexC, texC, colorC);
					if (getBlock(blockId, x, y, z - 1) == 0) drawDisplacedVertices(tvertices_negative_z, vertices_negative_z, absX, absY, absZ, bx, by, 0.65, vertexPtr, texPtr, colorPtr, vertexC, texC, colorC);

					bx = block->topTex.x;
					by = block->topTex.y;
					if (getBlock(blockId, x, y + 1, z) == 0) drawDisplacedVertices(tvertices_positive_y, vertices_positive_y, absX, absY, absZ, bx, by, 1.00, vertexPtr, texPtr, colorPtr, vertexC, texC, colorC);

					bx = block->bottomTex.x;
					by = block->bottomTex.y;
					if (getBlock(blockId, x, y - 1, z) == 0) drawDisplacedVertices(tvertices_negative_y, vertices_negative_y, absX, absY, absZ, bx, by, 0.20, vertexPtr, texPtr, colorPtr, vertexC, texC, colorC);
				}
			}
		}
	}
}

unsigned char SectionBuilder::getBlock(unsigned char me, int x, int y, int z) {
	unsigned char id = 0;
	if (x < 0 || y < 0 || z < 0 || x > 15 || y > 15 || z > 15) id = OpenGLRenderer::world->getBlock(xo + x, yo + y, zo + z);
	else id = chunk->getBlock(x, y, z);
	if ((!BlockRegistry::isFluid(me) && BlockRegistry::isFluid(id)) || BlockRegistry::isTransparent(me))
		return 0;
	else return id;
}