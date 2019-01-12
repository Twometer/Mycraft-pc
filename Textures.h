#pragma once
#include "Texture.h"
struct TEXTURES {
	TEXTURE top;
	TEXTURE side;
	TEXTURE bottom;

	TEXTURES() {

	}

	TEXTURES(int x, int y) {
		TEXTURE tex = TEXTURE(x, y);
		top = tex;
		side = tex;
		bottom = tex;
	}

	TEXTURES(TEXTURE all) {
		top = all;
		side = all;
		bottom = all;
	}

	TEXTURES(TEXTURE top, TEXTURE side, TEXTURE bottom) {
		this->top = top;
		this->side = side;
		this->bottom = bottom;
	}

};
