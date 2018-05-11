#pragma once
#include "stdafx.h"
class CrosshairRenderer
{
private:
	
	GLuint vao;
	GLuint texture;
	void loadToVao();
public:
	void init();
	void render();
	CrosshairRenderer(GLuint texture);
	~CrosshairRenderer();
};

