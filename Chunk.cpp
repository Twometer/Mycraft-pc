#include "Chunk.h"
#include <vector>
#include "Section.h"
#include "OpenGLRenderer.h"

using namespace std;

Chunk::Chunk(int x, int z)
{
	this->x = x;
	this->z = z;
	this->sections = new Section*[16];
	for (int i = 0; i < 16; i++) *(sections + i) = nullptr;
}

void Chunk::render(bool transparencyPass) {
	for (unsigned int i = 0; i < 16; i++) {
		Section* sec = *(sections + i);
		if (sec != nullptr)
			sec->render(transparencyPass, OpenGLRenderer::frustum->CubeInFrustum((this->x << 4) + 8, (i << 4) + 8, (this->z << 4) + 8, 8));
	}
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

unsigned char Chunk::getBlock(int x, int y, int z) {
	if (x < 0 || y < 0 || z < 0 || x > 15 || y > 255 || z > 15) {
		printf("Tried to retrieve block outside the bounds %d %d %d", x ,y ,z);
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
						unsigned char b = (unsigned char)(((data->storage[idx + 1] & 255) << 8 | data->storage[idx] & 255) >> 4);
						if (b > 0)
						{
							sec->setBlock(x, y, z, b, false);
						}
						idx += 2;
					}
				}
			}

		}
	}
}

Chunk::~Chunk()
{
}
