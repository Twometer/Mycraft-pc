#include "PostProcessing.h"
#include <iostream>
#include "ImageRenderer.h"
#include "OpenGLRenderer.h"
#include <glm/glm.hpp>
#include "Settings.h"

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


void PostProcessing::doPostProc(GLuint worldTexture) {
	start();
	bool gui = OpenGLRenderer::guiRenderer->isGuiOpen();
	if (gui) {
		apply_fluid(worldTexture, &fbo1);
		apply_hgauss(fbo1.getColorTexture(), &fboDS1);
		apply_vgauss(fboDS1.getColorTexture(), nullptr);
	}else
	{
		if(Settings::BLOOM)
		{
			apply_fluid(worldTexture, &fbo1);
			apply_bright(fbo1.getColorTexture(), &fbo2);
			apply_hgauss(fbo2.getColorTexture(), &fboDS1);
			apply_vgauss(fboDS1.getColorTexture(), &fboDS2);
			apply_mix(fbo1.getColorTexture(), fboDS2.getColorTexture(), nullptr);
		}else
		{
			apply_fluid(worldTexture, nullptr);
		}
	}
	stop();
}

void PostProcessing::apply_fluid(GLuint colortex, Fbo* fbo)
{
	vec3 eyePos = OpenGLRenderer::controls->getEyePosition();
	char blockInEyes = OpenGLRenderer::world->getBlock(floor(eyePos.x), floor(eyePos.y), floor(eyePos.z));
	int fluidMode = 0;
	if (blockInEyes == 8 || blockInEyes == 9) fluidMode = 1;
	else if (blockInEyes == 10 || blockInEyes == 11) fluidMode = 2;
	glUseProgram(shader_fluid);
	glUniform1i(shader_fluid_loc, fluidMode);
	if (fbo != nullptr) fbo->bindFrameBuffer();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colortex);
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	if (fbo != nullptr) fbo->unbindFrameBuffer();
}

void PostProcessing::apply_hgauss(GLuint colortex, Fbo* fbo)
{
	glUseProgram(shader_hgauss);
	glUniform1f(shader_hgauss_loc, curWidth / 5);
	if (fbo != nullptr) fbo->bindFrameBuffer();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colortex);
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	if (fbo != nullptr) fbo->unbindFrameBuffer();
}

void PostProcessing::apply_vgauss(GLuint colortex, Fbo* fbo)
{
	glUseProgram(shader_vgauss);
	glUniform1f(shader_vgauss_loc, curHeight / 5);
	if (fbo != nullptr) fbo->bindFrameBuffer();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colortex);
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	if (fbo != nullptr) fbo->unbindFrameBuffer();
}

void PostProcessing::apply_bright(GLuint colortex, Fbo* fbo)
{
	glUseProgram(shader_bright);
	if (fbo != nullptr) fbo->bindFrameBuffer();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colortex);
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	if (fbo != nullptr) fbo->unbindFrameBuffer();
}

void PostProcessing::apply_mix(GLuint colorTex, GLuint colorTex2, Fbo* fbo)
{
	glUseProgram(shader_mix);
	if (fbo != nullptr) fbo->bindFrameBuffer();
	glClear(GL_COLOR_BUFFER_BIT);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, colorTex);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, colorTex2);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	if (fbo != nullptr) fbo->unbindFrameBuffer();
}

void PostProcessing::init(Loader loader) {
	loadToVao(POSITIONS);
	shader_fluid = loader.loadShaders("postproc");
	shader_fluid_loc = glGetUniformLocation(shader_fluid, "fluidMode");

	shader_hgauss = loader.loadShaders("hgauss");
	shader_hgauss_loc = glGetUniformLocation(shader_hgauss, "targetWidth");

	shader_vgauss = loader.loadShaders("vgauss");
	shader_vgauss_loc = glGetUniformLocation(shader_vgauss, "targetHeight");

	shader_bright = loader.loadShaders("bright");
	shader_mix = loader.loadShaders("mix");
	GLint location_colorTexture = glGetUniformLocation(shader_mix, "colourTexture");
	GLint location_highlightTexture = glGetUniformLocation(shader_mix, "highlightTexture");
	glUseProgram(shader_mix);
	glUniform1i(location_colorTexture, 0);
	glUniform1i(location_highlightTexture, 1);
	glUseProgram(0);
}

void PostProcessing::resize(int width, int height)
{
	fbo1 = Fbo(width, height, NONE);
	fbo2 = Fbo(width, height, NONE);
	fbo3 = Fbo(width, height, NONE);

	fboDS1 = Fbo(width / 5, height / 5, NONE);
	fboDS2 = Fbo(width / 5, height / 5, NONE);
	curWidth = width;
	curHeight = height;
}

PostProcessing::PostProcessing()
{

}


PostProcessing::~PostProcessing()
{
}
