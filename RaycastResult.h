#pragma once
enum FACE {
	XPOS,
	XNEG,
	YPOS,
	YNEG,
	ZPOS,
	ZNEG,
	NO_RESULT
};
struct RAYCAST_RESULT {
	int blockX;
	int blockY;
	int blockZ;
	FACE face;
};

