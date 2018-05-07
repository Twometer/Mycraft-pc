#pragma once
#include "Loader.h"
class Skybox
{
private:
	GLuint vao;
public:
	Skybox();
	~Skybox();
	void initialize(Loader loader);
	void render();
};

