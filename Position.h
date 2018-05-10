#pragma once
#include <glad/glad.h>
struct POSITION {
	int x;
	int y;
	int z;
	POSITION() {

	}

	POSITION(uint64_t pos) {
		int x = (int)(pos >> 38);
		int y = (int)(pos << 26 >> 52);
		int z = (int)(pos << 38 >> 38);
		if (x >= 33554432) { x -= 67108864; }
		if (y >= 2048) { y -= 4096; }
		if (z >= 33554432) { z -= 67108864; }
		this->x = x;
		this->y = y;
		this->z = z;
	}
};