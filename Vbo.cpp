#include "Vbo.h"
#include <glad/glad.h>
#include <cstdio>

Vbo::Vbo()
{
}


Vbo::~Vbo()
{
	destroy();
}

void Vbo::create(Mesh * mesh)
{
	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, mesh->getVerticesCount() * sizeof(GLfloat), mesh->vertices, GL_STATIC_DRAW);

	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, mesh->getColorsCount() * sizeof(GLfloat), mesh->colors, GL_STATIC_DRAW);

	GLuint texturebuffer;
	glGenBuffers(1, &texturebuffer);
	glBindBuffer(GL_ARRAY_BUFFER, texturebuffer);
	glBufferData(GL_ARRAY_BUFFER, mesh->getTexCoordsCount() * sizeof(GLfloat), mesh->texCoords, GL_STATIC_DRAW);

	this->vertexBuffer = vertexbuffer;
	this->colorBuffer = colorbuffer;
	this->texCoordBuffer = texturebuffer;

	this->triangles = mesh->getVerticesCount() / 3;
	filled = true;
}

void Vbo::destroy()
{
	if (!filled)
		return;
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &colorBuffer);
	glDeleteBuffers(1, &texCoordBuffer);
	filled = false;
}

void Vbo::draw() 
{
	if (!filled)
		return;

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, texCoordBuffer);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glDrawArrays(GL_TRIANGLES, 0, triangles);
}

bool Vbo::isFilled()
{
	return this->filled;
}
