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
	this->vertices = new GLfloat[50000];
	this->colors = new GLfloat[50000];
	this->textureCoords = new GLfloat[50000];
}

SectionBuilder::~SectionBuilder()
{
	delete[] vertices;
	delete[] colors;
	delete[] textureCoords;
}

void SectionBuilder::drawDisplacedVertices(const GLfloat* textures, const GLfloat* vertices, int x, int y, int z, int texX, int texY, GLfloat col) {

	for (int i = 0; i < 18; i += 3) {
		*(this->vertices + (this->verticesAlloc++)) = *(vertices + i) + x;
		*(this->vertices + (this->verticesAlloc++)) = *(vertices + i + 1) + y;
		*(this->vertices + (this->verticesAlloc++)) = *(vertices + i + 2) + z;
		*(this->colors + (this->colorsAlloc++)) = col;
		*(this->colors + (this->colorsAlloc++)) = col;
		*(this->colors + (this->colorsAlloc++)) = col;
	}

	GLfloat d = 0.03125;

	for (int i = 0; i < 12; i += 2) {
		*(this->textureCoords + (this->textureCoordsAlloc++)) = (*(textures + i)) * d + texX * d;
		*(this->textureCoords + (this->textureCoordsAlloc++)) = (*(textures + i + 1)) * d + texY * d;
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
				if (BlockRegistry::isTransparent(blockId))continue;
				if (blockId != 0) {
					Block* block = BlockRegistry::getBlock(blockId);
					
					int bx = block->sideTex.x;
					int by = block->sideTex.y;
					if (getBlock(blockId, x + 1, y, z) == 0) drawDisplacedVertices(tvertices_positive_x, vertices_positive_x, absX, absY, absZ, bx, by, 0.75);
					if (getBlock(blockId, x - 1, y, z) == 0) drawDisplacedVertices(tvertices_negative_x, vertices_negative_x, absX, absY, absZ, bx, by, 0.75);

					if (getBlock(blockId, x, y, z + 1) == 0) drawDisplacedVertices(tvertices_positive_z, vertices_positive_z, absX, absY, absZ, bx, by, 0.65);
					if (getBlock(blockId, x, y, z - 1) == 0) drawDisplacedVertices(tvertices_negative_z, vertices_negative_z, absX, absY, absZ, bx, by, 0.65);

					bx = block->topTex.x;
					by = block->topTex.y;
					if (getBlock(blockId, x, y + 1, z) == 0) drawDisplacedVertices(tvertices_positive_y, vertices_positive_y, absX, absY, absZ, bx, by, 1.00);

					bx = block->bottomTex.x;
					by = block->bottomTex.y;
					if (getBlock(blockId, x, y - 1, z) == 0) drawDisplacedVertices(tvertices_negative_y, vertices_negative_y, absX, absY, absZ, bx, by, 0.20);
				}
			}
		}
	}
}

unsigned char SectionBuilder::getBlock(unsigned char me, int x, int y, int z) {
	unsigned char id = 0;
	if (x < 0 || y < 0 || z < 0 || x > 15 || y > 15 || z > 15) id = OpenGLRenderer::world->getBlock(xo + x, yo + y, zo + z);
	else id = chunk->getBlock(x, y, z);
	if ((!BlockRegistry::isTransparent(me) && BlockRegistry::isTransparent(id)) || BlockRegistry::isSpecialTransparent(me))
		return 0;
	else return id;
}