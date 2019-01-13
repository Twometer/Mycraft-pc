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
	0.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 0.0f
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
	0.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 0.0f
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
	0.0f, 0.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f
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
	1.0f, 1.0f,
	0.0f, 0.0f,
	0.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,
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
	0.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 0.0f,
	1.0f, 1.0f
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
	0.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 0.0f,
	1.0f, 1.0f
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

static const GLfloat flatvertices[] = {
	1.0f, 0.05f, 1.0f,
	1.0f, 0.05f, 0.0f,
	0.0f, 0.05f, 0.0f,
	1.0f, 0.05f, 1.0f,
	0.0f, 0.05f, 0.0f,
	0.0f, 0.05f, 1.0f
};

static const GLfloat tflatvertices[] = {
	0.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 0.0f,
	1.0f, 1.0f
};

static const GLfloat vertices_flat_side[] = {
	0.05f, 0.0f, 0.05f, // Pos x
	0.05f, 1.0f, 0.95f,
	0.05f, 0.0f, 0.95f,
	0.05f, 1.0f, 0.95f, // Pos x
	0.05f, 0.0f, 0.05f,
	0.05f, 1.0f, 0.05f,
	0.05f, 0.0f, 0.05f, // Neg x
	0.05f, 0.0f, 0.95f,
	0.05f, 1.0f, 0.95f,
	0.05f, 0.0f, 0.05f, // Neg x
	0.05f, 1.0f, 0.95f,
	0.05f, 1.0f, 0.05f
};

static const GLfloat tvertices_flat_side[] = {
	1.0f, 1.0f,
	0.0f, 0.0f,
	0.0f, 1.0f,
	0.0f, 0.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 0.0f
};

SectionBuilder::SectionBuilder(Section* chunk)
{
	this->chunk = chunk;
	this->regularMesh = new Mesh(120000);
	if (chunk->hasFluid)
		this->transparentMesh = new Mesh(30000);
}

SectionBuilder::~SectionBuilder()
{
	delete regularMesh;
	delete transparentMesh;
}

bool canOcclude(int x, int y, int z) {
	const unsigned char blockId = OpenGLRenderer::world->getBlock(x, y, z);
	const unsigned char meta = OpenGLRenderer::world->getMeta(x, y, z);
	Block* block = BlockRegistry::getBlock(blockId);
	return blockId && block->rendererType != Plant && block->canOcclude && block->getYOffset(meta) == 0.0f;
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

void SectionBuilder::drawDisplacedVertices(const GLfloat* textures, const GLfloat* vertices, int x, int y, int z, int texX, int texY, GLfloat col, Mesh* target, FACE face, int side, float height, float yOffset, int meta, VertexHandler* vertexHandler, RendererType r) {

	int totalV = r == FlatSide ? 36 : 18;
	int totalT = r == FlatSide ? 24 : 12;
	for (int i = 0; i < totalV; i += 3) {
		GLfloat vx = *(vertices + i);
		GLfloat vy = *(vertices + i + 1);
		GLfloat vz = *(vertices + i + 2);
		vy *= height;
		GLfloat occlusionModifier = height != 1 ? 1.0f : getOcclusionFactor(x, y, z, (int)vx, (int)vy, (int)vz, side);
		if (vertexHandler == nullptr)
			target->addVertex(vx + x, vy + y + yOffset, vz + z);
		else {
			glm::vec3 v = vertexHandler->process(meta, vx, vy, vz, glm::vec3(x, y, z), face);
			target->addVertex(v.x + x, v.y + y + yOffset, v.z + z);
		}

		target->addColor(occlusionModifier * col, occlusionModifier * col, occlusionModifier * col);
	}

	GLfloat d = 0.03125;

	for (int i = 0; i < totalT; i += 2) {
		if (vertexHandler == nullptr)
			target->addTexture(textures[i] * d + texX * d, side != 1 ? textures[i + 1] * height * d + texY * d : textures[i + 1] * d + texY * d);
		else {
			glm::vec2 tex = vertexHandler->processTex(textures[i], textures[i + 1], face);
			target->addTexture(tex.x * d + texX * d, side != 1 ? tex.y * height * d + texY * d : tex.y * d + texY * d);
		}
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
				const unsigned char blockId = chk.getBlock(x, y, z);
				if (blockId == 0)
					continue;
				unsigned char meta = chk.getMeta(x, y, z);
				Block* block = BlockRegistry::getBlock(blockId);
				float yo = block->getYOffset(meta);
				if (block->specialBlock == Slab && meta > 7)
					meta -= 8;
				TEXTURES textures = *block->getTextures(meta);

				const bool isTransparent = block->rendererType == Fluid;
				Mesh* mesh = isTransparent ? transparentMesh : regularMesh;

				int bx = textures.side.x;
				int by = textures.side.y;
				float ym = block->blockHeight;
				VertexHandler* vertexHandler = block->vertexHandler;

				RendererType rendererType = block->rendererType;

				//GLfloat light = absY < chk.parent->skylight[x * 16 + z] ? 0.5f : 1.0f;

				if (rendererType == Plant) {
					drawDisplacedVertices(tgrassvertices_a, grassvertices_a, absX, absY, absZ, bx, by, 1.0f, mesh, NO_RESULT, 0, ym, yo, meta, vertexHandler, rendererType);
					drawDisplacedVertices(tgrassvertices_b, grassvertices_b, absX, absY, absZ, bx, by, 1.0f, mesh, NO_RESULT, 0, ym, yo, meta, vertexHandler, rendererType);
					drawDisplacedVertices(tgrassvertices_c, grassvertices_c, absX, absY, absZ, bx, by, 1.0f, mesh, NO_RESULT, 0, ym, yo, meta, vertexHandler, rendererType);
					drawDisplacedVertices(tgrassvertices_d, grassvertices_d, absX, absY, absZ, bx, by, 1.0f, mesh, NO_RESULT, 0, ym, yo, meta, vertexHandler, rendererType);
				}
				else if (rendererType == Flat) {
					drawDisplacedVertices(tflatvertices, flatvertices, absX, absY, absZ, bx, by, 1.0f, mesh, NO_RESULT, 1, ym, yo, meta, vertexHandler, rendererType);
				}
				else if (rendererType == FlatSide) {
					drawDisplacedVertices(tvertices_flat_side, vertices_flat_side, absX, absY, absZ, bx, by, 1.0f, mesh, NO_RESULT, 1, ym, yo, meta, vertexHandler, rendererType);
				}
				else {
					if (getBlock(block, x + 1, y, z, 0) == 0) drawDisplacedVertices(tvertices_positive_x, vertices_positive_x, absX, absY, absZ, bx, by, 0.85f, mesh, XPOS, 0, ym, yo, meta, vertexHandler, rendererType);
					if (getBlock(block, x - 1, y, z, 1) == 0) drawDisplacedVertices(tvertices_negative_x, vertices_negative_x, absX, absY, absZ, bx, by, 0.85f, mesh, XNEG, 0, ym, yo, meta, vertexHandler, rendererType);

					if (getBlock(block, x, y, z + 1, 2) == 0) drawDisplacedVertices(tvertices_positive_z, vertices_positive_z, absX, absY, absZ, bx, by, 0.65f, mesh, ZPOS, 2, ym, yo, meta, vertexHandler, rendererType);
					if (getBlock(block, x, y, z - 1, 3) == 0) drawDisplacedVertices(tvertices_negative_z, vertices_negative_z, absX, absY, absZ, bx, by, 0.65f, mesh, ZNEG, 2, ym, yo, meta, vertexHandler, rendererType);

					bx = textures.top.x;
					by = textures.top.y;
					if (getBlock(block, x, y + 1, z, 4) == 0) drawDisplacedVertices(tvertices_positive_y, vertices_positive_y, absX, absY, absZ, bx, by, 1.00f, mesh, YPOS, 1, ym, yo, meta, vertexHandler, rendererType);

					bx = textures.bottom.x;
					by = textures.bottom.y;
					if (getBlock(block, x, y - 1, z, 5) == 0) drawDisplacedVertices(tvertices_negative_y, vertices_negative_y, absX, absY, absZ, bx, by, 0.50f, mesh, YNEG, 1, ym, yo, meta, vertexHandler, rendererType);
				}
			}
		}
	}
}

unsigned char SectionBuilder::getBlock(Block* me, int x, int y, int z, int f) {
	unsigned char id = 0;
	if (x < 0 || y < 0 || z < 0 || x > 15 || y > 15 || z > 15) id = OpenGLRenderer::world->getBlock(xo + x, yo + y, zo + z);
	else id = chunk->getBlock(x, y, z);
	if (id == 0)
		return 0;
	Block* other = BlockRegistry::getBlock(id);
	if (f == 4 && me->rendererType == Fluid && other->rendererType != Fluid)
		return 0;
	if ((!me->rendererType == Fluid && other->rendererType == Fluid) || me->isTransparent() || other->isTransparent())
		return 0;
	return id;
}
