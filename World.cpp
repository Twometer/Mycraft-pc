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
			chk->setBlock(x - (chunkX << 4), y, z - (chunkZ << 4), id, true);
		}
	}
}

void World::render(bool transparencyPass) {
	glm::vec3 pos = OpenGLRenderer::controls->getPosition();
	int pcx = (int)pos.x >> 4;
	int pcz = (int)pos.z >> 4;
	for (unsigned int i = 0; i < chunkLen; i++) {
		Chunk* chk = *(chunkArray + i);
		if (chk != nullptr && abs(chk->x - pcx) <= Settings::RENDER_DISTANCE && abs(chk->z - pcz) <= Settings::RENDER_DISTANCE) {
			chk->render(transparencyPass);
		}
	}
}

void World::addChunk(Chunk *chk) {
	mtx.lock();
	*(chunkArray + chunkLen) = chk;
	chunkLen++;
	mtx.unlock();
}

void World::deleteChunk(int x, int z)
{
	for (unsigned int i = 0; i < chunkLen; i++)
	{
		Chunk* chk = chunkArray[i];
		if (chk != nullptr && chk->x == x && chk->z == z)
		{
			chk->destroy();
			chunkArray[i] = nullptr;
			delete chk;
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
				if (bid > 0 && bid != 8 && bid != 9 && bid != 31 && bid != 175 && bid != 10 && bid != 11 && !BlockRegistry::isPlant(bid)) {
					cubes.push_back(AABB(glm::vec3(xx + x, xy + y, xz + z), glm::vec3(xx + x, xy + y, xz + z)).expand(1.0, bid == 78 ? 0.1 : 1.0, 1.0));
				}
			}
		}
	}
	return cubes;
}
