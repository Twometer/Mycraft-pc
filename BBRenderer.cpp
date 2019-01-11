#include "BBRenderer.h"
#include <iostream>
#include <glad/glad.h>

static const GLfloat BB_VERTICES[] = {
	-0.001,  1.001,-0.001,
	-0.001,-0.001,-0.001,
	1.001,-0.001,-0.001,
	1.001,-0.001,-0.001,
	1.001,  1.001,-0.001,
	-0.001,  1.001,-0.001,

	-0.001,-0.001,  1.001,
	-0.001,-0.001,-0.001,
	-0.001,  1.001,-0.001,
	-0.001,  1.001,-0.001,
	-0.001,  1.001,  1.001,
	-0.001,-0.001,  1.001,

	1.001,-0.001,-0.001,
	1.001,-0.001,  1.001,
	1.001,  1.001,  1.001,
	1.001,  1.001,  1.001,
	1.001,  1.001,-0.001,
	1.001,-0.001,-0.001,

	-0.001,-0.001,  1.001,
	-0.001,  1.001,  1.001,
	1.001,  1.001,  1.001,
	1.001,  1.001,  1.001,
	1.001,-0.001,  1.001,
	-0.001,-0.001,  1.001,

	-0.001,  1.001,-0.001,
	1.001,  1.001,-0.001,
	1.001,  1.001,  1.001,
	1.001,  1.001,  1.001,
	-0.001,  1.001,  1.001,
	-0.001,  1.001,-0.001,

	-0.001,-0.001,-0.001,
	-0.001,-0.001,  1.001,
	1.001,-0.001,-0.001,
	1.001,-0.001,-0.001,
	-0.001,-0.001,  1.001,
	1.001,-0.001,  1.001
};

BBRenderer::BBRenderer()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 108, BB_VERTICES, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}


BBRenderer::~BBRenderer()
{

}

void BBRenderer::render() {
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
}