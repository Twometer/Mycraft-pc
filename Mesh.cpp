#include "Mesh.h"


Mesh::Mesh(int capacity)
{
	capacity3 = capacity;
	capacity2 = capacity / 3 * 2;
	vertices = new GLfloat[capacity3];
	colors = new GLfloat[capacity3];
	texCoords = new GLfloat[capacity2];
}


Mesh::~Mesh()
{
	delete[] vertices;
	delete[] colors;
	delete[] texCoords;
}

void Mesh::addVertex(GLfloat x, GLfloat y, GLfloat z)
{
	if (verticesIdx + 3 >= capacity3)
		return;
	vertices[verticesIdx] = x;
	vertices[verticesIdx + 1] = y;
	vertices[verticesIdx + 2] = z;
	verticesIdx += 3;
}

void Mesh::addColor(GLfloat r, GLfloat g, GLfloat b)
{
	if (colorsIdx + 3 >= capacity3)
		return;
	colors[colorsIdx] = r;
	colors[colorsIdx + 1] = g;
	colors[colorsIdx + 2] = b;
	colorsIdx += 3;
}

void Mesh::addTexture(GLfloat u, GLfloat v)
{
	if (texCoordsIdx + 2 >= capacity2)
		return;
	texCoords[texCoordsIdx] = u;
	texCoords[texCoordsIdx + 1] = v;
	texCoordsIdx += 2;
}

int Mesh::getVerticesCount()
{
	return verticesIdx;
}

int Mesh::getColorsCount()
{
	return colorsIdx;
}

int Mesh::getTexCoordsCount()
{
	return texCoordsIdx;
}
