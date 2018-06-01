#include "SectionBuilder.h"
#include <vector>
#include "GLFW/glfw3.h"
#include <stdio.h>
#include "OpenGLRenderer.h"
#include "BlockRegistry.h"
#include <iostream>
#include "Settings.h"

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

static const GLfloat grassvertices_a[] = {
	0.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 1.0f,

	0.0f, 0.0f, 0.0f,
	1.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 0.0f
};

static const GLfloat tgrassvertices_a[] = {
	0.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,

	0.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 0.0f
};

static const GLfloat grassvertices_b[] = {
	0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 1.0f, 0.0f,

	0.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 1.0f
};

static const GLfloat tgrassvertices_b[] = {
	0.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,

	0.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 0.0f
};

static const GLfloat grassvertices_c[] = {
	1.0f, 1.0f, 1.0f,
	1.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 0.0f,

	0.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 1.0f,
	0.0f, 0.0f, 0.0f,
};

static const GLfloat tgrassvertices_c[] = {
	1.0f, 0.0f,
	1.0f, 1.0f,
	0.0f, 1.0f,

	0.0f, 0.0f,
	1.0f, 0.0f,
	0.0f, 1.0f
};

static const GLfloat grassvertices_d[] = {
	1.0f, 1.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f,

	0.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 1.0f
};

static const GLfloat tgrassvertices_d[] = {
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
	this->vertices = new GLfloat[120000]; // Three entries per vertex
	this->colors = new GLfloat[120000]; // Three entries per vertex
	this->textureCoords = new GLfloat[90000]; // Two entries per vertex

	this->verticesX = new GLfloat[30000]; // 3
	this->colorsX = new GLfloat[30000]; // 3
	this->textureCoordsX = new GLfloat[22800]; // 2
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

bool canOcclude(int x, int y, int z) {
	unsigned char block = OpenGLRenderer::world->getBlock(x, y, z);
	return block && !BlockRegistry::isPlant(block) && block != 78;
}

float getOcclusionFactor(int x, int y, int z, int vx, int vy, int vz, int f) {
	if (!Settings::AMBIENT_OCCLUSION) return 1.0f;
	if (vx == 0) vx = -1;
	if (vy == 0) vy = -1;
	if (vz == 0) vz = -1;

	if (f == 0) {
		float oc = 1.0f;
		if (canOcclude(x + vx, y + vy, z)) oc -= 0.2;
		if (canOcclude(x + vx, y + vy, z + vz)) oc -= 0.2;
		if (canOcclude(x + vx, y, z + vz)) oc -= 0.2;
		return oc;
	}

	if (f == 1) {
		float oc = 1.0f;
		if (canOcclude(x + vx, y + vy, z)) oc -= 0.2;
		if (canOcclude(x, y + vy, z + vz)) oc -= 0.2;
		if (canOcclude(x + vx, y + vy, z + vz)) oc -= 0.2;
		return oc;
	}

	if (f == 2) {
		float oc = 1.0f;
		if (canOcclude(x + vx, y, z + vz)) oc -= 0.2;
		if (canOcclude(x, y + vy, z + vz)) oc -= 0.2;
		if (canOcclude(x + vx, y + vy, z + vz)) oc -= 0.2;
		return oc;
	}
	return 1.0f;
}

void SectionBuilder::drawDisplacedVertices(const GLfloat* textures, const GLfloat* vertices, int x, int y, int z, int texX, int texY, GLfloat col, GLfloat* vertexPtr, GLfloat* texPtr, GLfloat* colorPtr, int* vertexC, int* texC, int * colorC, int f, float ym) {

	for (int i = 0; i < 18; i += 3) {
		GLfloat vx = *(vertices + i);
		GLfloat vy = *(vertices + i + 1);
		GLfloat vz = *(vertices + i + 2);
		vy *= ym;
		GLfloat colx = ym != 1 ? 1.0f : getOcclusionFactor(x, y, z, (int)vx, (int)vy, (int)vz, f);
		*(vertexPtr + ((*vertexC)++)) = vx + x;
		*(vertexPtr + ((*vertexC)++)) = vy + y;
		*(vertexPtr + ((*vertexC)++)) = vz + z;
		*(colorPtr + ((*colorC)++)) = colx * col;
		*(colorPtr + ((*colorC)++)) = colx * col;
		*(colorPtr + ((*colorC)++)) = colx * col;
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
					float ym = blockId == 78 ? 0.1f : 1.0f;
					if (BlockRegistry::isPlant(blockId)) {
						drawDisplacedVertices(tgrassvertices_a, grassvertices_a, absX, absY, absZ, bx, by, 1.0f, vertexPtr, texPtr, colorPtr, vertexC, texC, colorC, 0, ym);
						drawDisplacedVertices(tgrassvertices_b, grassvertices_b, absX, absY, absZ, bx, by, 1.0f, vertexPtr, texPtr, colorPtr, vertexC, texC, colorC, 0, ym);
						drawDisplacedVertices(tgrassvertices_c, grassvertices_c, absX, absY, absZ, bx, by, 1.0f, vertexPtr, texPtr, colorPtr, vertexC, texC, colorC, 0, ym);
						drawDisplacedVertices(tgrassvertices_d, grassvertices_d, absX, absY, absZ, bx, by, 1.0f, vertexPtr, texPtr, colorPtr, vertexC, texC, colorC, 0, ym);
					}
					else {
						if (getBlock(blockId, x + 1, y, z) == 0) drawDisplacedVertices(tvertices_positive_x, vertices_positive_x, absX, absY, absZ, bx, by, 0.75f, vertexPtr, texPtr, colorPtr, vertexC, texC, colorC, 0, ym);
						if (getBlock(blockId, x - 1, y, z) == 0) drawDisplacedVertices(tvertices_negative_x, vertices_negative_x, absX, absY, absZ, bx, by, 0.75f, vertexPtr, texPtr, colorPtr, vertexC, texC, colorC, 0, ym);

						if (getBlock(blockId, x, y, z + 1) == 0) drawDisplacedVertices(tvertices_positive_z, vertices_positive_z, absX, absY, absZ, bx, by, 0.65f, vertexPtr, texPtr, colorPtr, vertexC, texC, colorC, 2, ym);
						if (getBlock(blockId, x, y, z - 1) == 0) drawDisplacedVertices(tvertices_negative_z, vertices_negative_z, absX, absY, absZ, bx, by, 0.65f, vertexPtr, texPtr, colorPtr, vertexC, texC, colorC, 2, ym);

						bx = block->topTex.x;
						by = block->topTex.y;
						if (getBlock(blockId, x, y + 1, z) == 0) drawDisplacedVertices(tvertices_positive_y, vertices_positive_y, absX, absY, absZ, bx, by, 1.00f, vertexPtr, texPtr, colorPtr, vertexC, texC, colorC, 1, ym);

						bx = block->bottomTex.x;
						by = block->bottomTex.y;
						if (getBlock(blockId, x, y - 1, z) == 0) drawDisplacedVertices(tvertices_negative_y, vertices_negative_y, absX, absY, absZ, bx, by, 0.60f, vertexPtr, texPtr, colorPtr, vertexC, texC, colorC, 1, ym);
					}
				}
			}
		}
	}
}

unsigned char SectionBuilder::getBlock(unsigned char me, int x, int y, int z) {
	unsigned char id = 0;
	if (x < 0 || y < 0 || z < 0 || x > 15 || y > 15 || z > 15) id = OpenGLRenderer::world->getBlock(xo + x, yo + y, zo + z);
	else id = chunk->getBlock(x, y, z);
	if ((!BlockRegistry::isFluid(me) && BlockRegistry::isFluid(id)) || BlockRegistry::isTransparent(me) || BlockRegistry::isTransparent(id))
		return 0;
	else return id;
}