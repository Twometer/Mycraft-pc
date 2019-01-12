#pragma once
#include "GLFW/glfw3.h"
class Mesh
{
private:
	int capacity3;
	int capacity2;

	int verticesIdx;
	int colorsIdx;
	int texCoordsIdx;
public:
	GLfloat* vertices;
	GLfloat* colors;
	GLfloat* texCoords;

	Mesh(int capacity);
	~Mesh();
	void addVertex(GLfloat x, GLfloat y, GLfloat z);
	void addColor(GLfloat r, GLfloat g, GLfloat b);
	void addTexture(GLfloat u, GLfloat v);

	int getVerticesCount();
	int getColorsCount();
	int getTexCoordsCount();
};

