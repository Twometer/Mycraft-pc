#include "World.h"
#include "concurrent_vector.h"
#include "Chunk.h"
#include <mutex>

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
			return chk->getBlock(x - (chunkX << 4), y, z - (chunkZ << 4));
		}
	}
	return 0;
}

void World::setBlock(int x, int y, int z, unsigned char id)
{

}

void World::render() {
	for (unsigned int i = 0; i < chunkLen; i++) {
		Chunk* chk = *(chunkArray + i);
		if (chk != nullptr)
			chk->render();
	}
}

void World::addChunk(Chunk *chk) {
	mtx.lock();
	*(chunkArray + chunkLen) = chk;
	chunkLen++;
	mtx.unlock();
}