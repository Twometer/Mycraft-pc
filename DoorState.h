#pragma once

enum Half {
	UPPER,
	LOWER
};

enum Facing {
	POSX = 0,
	POSZ = 1,
	NEGX = 2,
	NEGZ = 3
};

struct DOORSTATE {
	Half half;
	Facing facing;
	bool open;

	DOORSTATE(int meta) {
		half = (meta & 0b00001000) != 0 ? UPPER : LOWER;
		open = (meta & 0b00000100) != 0;
		facing = static_cast<Facing>(meta & 0b00000011);
	}

};