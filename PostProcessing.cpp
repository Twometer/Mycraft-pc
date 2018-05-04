#include "PostProcessing.h"
#include <iostream>

GLfloat POSITIONS[] = { -1, 1, -1, -1, 1, 1, 1, -1 };
const int AMOUNT = 8;

void PostProcessing::loadToVao(GLfloat* positions) {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * AMOUNT, positions, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, false, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void PostProcessing::start() {
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glDisable(GL_DEPTH_TEST);
}


void PostProcessing::stop() {
	glEnable(GL_DEPTH_TEST);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
}


void PostProcessing::doPostProc(GLuint colortex) {
	start();
	glUseProgram(shader);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colortex);
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	stop();
}

void PostProcessing::init() {
	loadToVao(POSITIONS);
}

PostProcessing::PostProcessing(GLuint shader)
{
	this->shader = shader;
}


PostProcessing::~PostProcessing()
{
}
