#pragma once
#include <glad/glad.h>
class BBRenderer
{
private:
	GLuint vao;
public:
	BBRenderer();
	~BBRenderer();
	void render();
};

