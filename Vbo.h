#pragma once
#include "Mesh.h"
class Vbo
{
private:
	GLuint vertexBuffer;
	GLuint colorBuffer;
	GLuint texCoordBuffer;

	int triangles;
	bool filled;

public:
	Vbo();
	~Vbo();

	void create(Mesh* mesh);
	void destroy();
	void draw();
	bool isFilled();
};

