#include "PostProcessing.h"
#include <iostream>
#include "ImageRenderer.h"
#include "OpenGLRenderer.h"
#include <glm/glm.hpp>

GLfloat POSITIONS[] = { -1, 1, -1, -1, 1, 1, 1, -1 };
const int AMOUNT = 8;

using namespace glm;

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
}


void PostProcessing::stop() {
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
}


void PostProcessing::doPostProc(GLuint colortex) {
	start();
	bool gui = OpenGLRenderer::guiRenderer->isGuiOpen();
	GLuint a = apply_fluid(colortex, gui);
	if (gui) {
		GLuint b = apply_hgauss(a, true);
		GLuint c = apply_vgauss(b, true);
		GLuint d = apply_hgauss(c, true);
		GLuint e = apply_vgauss(d, false);
	}
	stop();
}

GLuint PostProcessing::apply_fluid(GLuint colortex, bool toFbo)
{
	vec3 eyePos = OpenGLRenderer::controls->getEyePosition();
	char blockInEyes = OpenGLRenderer::world->getBlock(floor(eyePos.x), floor(eyePos.y), floor(eyePos.z));
	int fluidMode = 0;
	if (blockInEyes == 8 || blockInEyes == 9) fluidMode = 1;
	else if (blockInEyes == 10 || blockInEyes == 11) fluidMode = 2;
	glUseProgram(shader_fluid);
	glUniform1i(shader_fluid_loc, fluidMode);
	if (toFbo) fbo1.bindFrameBuffer();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colortex);
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	if (toFbo) fbo1.unbindFrameBuffer();
	return fbo1.getColorTexture();
}

GLuint PostProcessing::apply_hgauss(GLuint colortex, bool toFbo)
{
	glUseProgram(shader_hgauss);
	glUniform1f(shader_hgauss_loc, curWidth);
	if (toFbo) fbo2.bindFrameBuffer();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colortex);
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	if (toFbo) fbo2.unbindFrameBuffer();
	return fbo2.getColorTexture();
}

GLuint PostProcessing::apply_vgauss(GLuint colortex, bool toFbo)
{
	glUseProgram(shader_vgauss);
	glUniform1f(shader_vgauss_loc, curHeight);
	if (toFbo) fbo3.bindFrameBuffer();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colortex);
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	if (toFbo) fbo3.unbindFrameBuffer();
	return fbo3.getColorTexture();
}

void PostProcessing::init(Loader loader) {
	loadToVao(POSITIONS);
	shader_fluid = loader.loadShaders("postproc");
	shader_fluid_loc = glGetUniformLocation(shader_fluid, "fluidMode");

	shader_hgauss = loader.loadShaders("hgauss");
	shader_hgauss_loc = glGetUniformLocation(shader_hgauss, "targetWidth");

	shader_vgauss = loader.loadShaders("vgauss");
	shader_vgauss_loc = glGetUniformLocation(shader_vgauss, "targetHeight");
}

void PostProcessing::resize(int width, int height)
{
	fbo1 = Fbo(width, height, NONE);
	fbo2 = Fbo(width, height, NONE);
	fbo3 = Fbo(width, height, NONE);
	curWidth = width;
	curHeight = height;
}

PostProcessing::PostProcessing()
{

}


PostProcessing::~PostProcessing()
{
}
