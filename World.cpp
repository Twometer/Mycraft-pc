#include "World.h"
#include "concurrent_vector.h"
#include "Chunk.h"
#include <mutex>
#include "BlockRegistry.h"
#include "Settings.h"
#include "Controls.h"
#include "OpenGLRenderer.h"

using namespace std;

mutex mtx;

World::World()
{
	chunkArray = new Chunk*[2048];
}


World::~World()
{
}

unsigned char World::getMeta(int x, int y, int z)
{
	if (y < 0 || y > 255) return 0;
	int chunkX = x >> 4;
	int chunkZ = z >> 4;
	for (unsigned int i = 0; i < chunkLen; i++) {
		Chunk* chk = *(chunkArray + i);
		if (chk != nullptr && chk->x == chunkX && chk->z == chunkZ) {
			return chk->getMeta(x & 15, y, z & 15);
		}
	}
	return 0;
}

unsigned char World::getBlock(int x, int y, int z)
{
	if (y < 0 || y > 255) return 0;
	int chunkX = x >> 4;
	int chunkZ = z >> 4;
	for (unsigned int i = 0; i < chunkLen; i++) {
		Chunk* chk = *(chunkArray + i);
		if (chk != nullptr && chk->x == chunkX && chk->z == chunkZ) {
			return chk->getBlock(x & 15, y, z & 15);
		}
	}
	return 0;
}

void World::setBlock(int x, int y, int z, unsigned char id)
{
	if (y < 0 || y > 255) return;
	int chunkX = x >> 4;
	int chunkZ = z >> 4;
	for (unsigned int i = 0; i < chunkLen; i++) {
		Chunk* chk = *(chunkArray + i);
		if (chk != nullptr && chk->x == chunkX && chk->z == chunkZ) {
			chk->setBlock(x & 15, y, z & 15, id, true);
		}
	}
}

void World::setBlockAndMeta(int x, int y, int z, unsigned char id, unsigned char meta)
{
	if (y < 0 || y > 255) return;
	int chunkX = x >> 4;
	int chunkZ = z >> 4;
	for (unsigned int i = 0; i < chunkLen; i++) {
		Chunk* chk = *(chunkArray + i);
		if (chk != nullptr && chk->x == chunkX && chk->z == chunkZ) {
			chk->setBlockAndMeta(x & 15, y, z & 15, id, meta, true);
		}
	}
}

void World::render(bool transparencyPass, bool ignoreFrustum) {
	glm::vec3 pos = OpenGLRenderer::controls->getPosition();
	int pcx = (int)pos.x >> 4;
	int pcz = (int)pos.z >> 4;
	for (unsigned int i = 0; i < chunkLen; i++) {
		Chunk* chk = *(chunkArray + i);
		if (chk != nullptr) {
			if (abs(chk->x - pcx) <= Settings::RENDER_DISTANCE && abs(chk->z - pcz) <= Settings::RENDER_DISTANCE)
				chk->render(transparencyPass, ignoreFrustum);
		}
	}
}

void World::addChunk(Chunk *chk) {
	mtx.lock();
	*(chunkArray + chunkLen) = chk;
	chunkLen++;
	mtx.unlock();
}

void World::destroyChunk(int x, int z)
{
	for (unsigned int i = 0; i < chunkLen; i++)
	{
		Chunk* chk = chunkArray[i];
		if (chk != nullptr && chk->x == x && chk->z == z)
		{
			chk->destroy();
			return;
		}
	}
}

void World::deleteChunk(int x, int z)
{
	for (unsigned int i = 0; i < chunkLen; i++)
	{
		Chunk* chk = chunkArray[i];
		if (chk != nullptr && chk->x == x && chk->z == z)
		{
			chunkArray[i] = nullptr;
			return;
		}
	}
}

void World::reload()
{
	for (unsigned int i = 0; i < chunkLen; i++)
	{
		Chunk* chk = chunkArray[i];
		if (chk != nullptr)
		{
			chk->reload();
		}
	}
}

vector<AABB> World::getCubes(int xx, int xy, int xz, int r) {
	vector<AABB> cubes = vector<AABB>();
	for (int x = -r; x < r; x++)
	{
		for (int y = -r; y < r; y++)
		{
			for (int z = -r; z < r; z++)
			{
				unsigned char bid = getBlock(xx + x, xy + y, xz + z);
				if (bid == 0)
					continue;
				Block* block = BlockRegistry::getBlock(bid);
				float blockHeight = block->blockHeight;
				float yo = block->getYOffset(getMeta(xx + x, xy + y, xz + z));
				if (block->canCollide) {
					cubes.push_back(AABB(glm::vec3(xx + x, xy + y + yo, xz + z), glm::vec3(xx + x, xy + y + yo, xz + z)).expand(1.0, blockHeight, 1.0));
				}
			}
		}
	}
	return cubes;
}
