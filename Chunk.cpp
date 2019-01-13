#include "Chunk.h"
#include <vector>
#include "Section.h"
#include "OpenGLRenderer.h"
#include "BlockRegistry.h"

using namespace std;

Chunk::Chunk(int x, int z)
{
	this->x = x;
	this->z = z;
	this->worldX = x << 4;
	this->worldZ = z << 4;
	this->sections = new Section*[16];
	for (int i = 0; i < 16; i++) *(sections + i) = nullptr;
}

void Chunk::render(bool transparencyPass, bool ignoreFrustum) {
	if (deleted) return;
	for (unsigned int i = 0; i < 16; i++) {
		Section* sec = *(sections + i);
		if (sec != nullptr)
			sec->render(transparencyPass, ignoreFrustum || OpenGLRenderer::frustum->CubeInFrustum(worldX + 8, (i << 4) + 8, worldZ + 8, 8));
	}
}

void Chunk::check_deletion(bool transparencyPass)
{
	for (unsigned int i = 0; i < 16; i++) {
		Section* sec = *(sections + i);
		if (sec != nullptr)
			sec->check_deletion(transparencyPass);
	}
}

void Chunk::destroy()
{
	for (unsigned int i = 0; i < 16; i++) {
		Section* sec = *(sections + i);
		if (sec != nullptr)
			sec->destroy();
	}
}

void Chunk::destructionComplete(int idx)
{
	sections[idx] = nullptr;
	for (unsigned int i = 0; i < 16; i++) {
		Section* sec = *(sections + i);
		if (sec != nullptr)
			return;
	}
	OpenGLRenderer::world->deleteChunk(x, z);
}


void Chunk::buildVertexData() {
	for (unsigned int i = 0; i < 16; i++) {
		Section* sec = *(sections + i);
		if (sec != nullptr)
			sec->buildVertexData();
	}
}

void Chunk::uploadVertexData() {
	for (unsigned int i = 0; i < 16; i++) {
		Section* sec = *(sections + i);
		if (sec != nullptr)
			sec->uploadVertexData();
	}
}

void Chunk::setBlock(int x, int y, int z, unsigned char id, bool updateSection) {
	if (x < 0 || y < 0 || z < 0 || x > 15 || y > 255 || z > 15)
		return;
	int sectionIdx = y >> 4;
	Section* sec = *(sections + sectionIdx);
	if (sec == nullptr) {
		sec = new Section(this, this->x, sectionIdx, this->z);
		*(sections + sectionIdx) = sec;
	}
	sec->setBlock(x, y - (sectionIdx << 4), z, id, updateSection);
}

void Chunk::setBlockAndMeta(int x, int y, int z, unsigned char id, unsigned char meta, bool updateSection) {
	if (x < 0 || y < 0 || z < 0 || x > 15 || y > 255 || z > 15)
		return;
	int sectionIdx = y >> 4;
	Section* sec = *(sections + sectionIdx);
	if (sec == nullptr) {
		sec = new Section(this, this->x, sectionIdx, this->z);
		*(sections + sectionIdx) = sec;
	}
	int ty = y - (sectionIdx << 4);
	sec->setBlock(x, ty, z, id, false);
	sec->setMeta(x, ty, z, meta);
	sec->reload();
}

unsigned char Chunk::getMeta(int x, int y, int z) {
	if (x < 0 || y < 0 || z < 0 || x > 15 || y > 255 || z > 15) {
		printf("Tried to retrieve meta outside the bounds %d %d %d", x, y, z);
		return 0;
	}
	int sectionIdx = y >> 4;
	Section* sec = *(sections + sectionIdx);
	if (sec == nullptr)
		return 0;
	return sec->getMeta(x, y - (sectionIdx << 4), z);
}

unsigned char Chunk::getBlock(int x, int y, int z) {
	if (x < 0 || y < 0 || z < 0 || x > 15 || y > 255 || z > 15) {
		printf("Tried to retrieve block outside the bounds %d %d %d", x, y, z);
		return 0;
	}
	int sectionIdx = y >> 4;
	Section* sec = *(sections + sectionIdx);
	if (sec == nullptr)
		return 0;
	return sec->getBlock(x, y - (sectionIdx << 4), z);
}

void Chunk::initialize(ChunkExtracted* data) {
	short bitmask = data->bitmask;
	int idx = 0;
	for (int ct = 0; ct < 16; ct++)
	{
		if ((bitmask & (1 << ct)) != 0)
		{
			Section* sec = *(sections + ct);
			if (sec == nullptr) {
				sec = new Section(this, this->x, ct, this->z);
				*(sections + ct) = sec;
			}

			for (int y = 0; y < 16; y++)
			{
				for (int z = 0; z < 16; z++)
				{
					for (int x = 0; x < 16; x++)
					{
						unsigned short raw = ((data->storage[idx + 1] & 255) << 8 | data->storage[idx] & 255);
						unsigned char b = raw >> 4;
						unsigned char meta = raw & 15;
						if (b != 0)
						{
							sec->setBlock(x, y, z, b, false);
							if (!sec->hasFluid && BlockRegistry::getBlock(b)->rendererType == Fluid) {
								sec->hasFluid = true;
							}
							if (meta != 0)
								sec->setMeta(x, y, z, meta);
						}
						idx += 2;
					}
				}
			}

		}
	}

	/*for (int i = 0; i < 16; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			for (int k = 255; k >= 0; k--)
			{
				if (getBlock(i, k, j))
				{
					skylight[i * 16 + k] = k;
					break;
				}
			}
		}
	}*/
}

void Chunk::reload()
{
	for (unsigned int i = 0; i < 16; i++) {
		Section* sec = *(sections + i);
		if (sec != nullptr)
			sec->reload();
	}
}

Chunk::~Chunk()
{
}
