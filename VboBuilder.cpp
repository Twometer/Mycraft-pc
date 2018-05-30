#include "VboBuilder.h"



VboBuilder::VboBuilder(int dimen)
{
	this->dimen = dimen;
}


VboBuilder::~VboBuilder()
{
}

void VboBuilder::vertex3(int x, int y, int z)
{
	vertices.push_back((GLfloat)x);
	vertices.push_back((GLfloat)y);
	vertices.push_back((GLfloat)z);
}

void VboBuilder::vertex2(int x, int y)
{
	vertices.push_back((GLfloat)x);
	vertices.push_back((GLfloat)y);
}

void VboBuilder::color(int r, int g, int b, int a)
{
	colors.push_back((GLfloat)r / 255.0f);
	colors.push_back((GLfloat)g / 255.0f);
	colors.push_back((GLfloat)b / 255.0f);
	colors.push_back((GLfloat)a / 255.0f);
	hasColor = true;
}

void VboBuilder::drawRect(int x, int y, int w, int h, COLORDATA c)
{
	color(c.r, c.g, c.b, c.a);
	vertex2(x, y + h);

	color(c.r, c.g, c.b, c.a);
	vertex2(x, y);

	color(c.r, c.g, c.b, c.a);
	vertex2(x + w, y + h);

	color(c.r, c.g, c.b, c.a);
	vertex2(x + w, y);
}

void VboBuilder::build()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vertices.size(), &vertices.front(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, 0);

	if (hasColor) {
		GLuint colorBuffer;
		glGenBuffers(1, &colorBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * colors.size(), &colors.front(), GL_STATIC_DRAW);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
	}

	glBindVertexArray(0);
}

void VboBuilder::render()
{
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	if(hasColor) glEnableVertexAttribArray(1);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size() / dimen);

	if (hasColor) glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
	
	glBindVertexArray(0);
}

void VboBuilder::buildAndRender() {
	build();
	render();
}