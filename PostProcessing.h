#pragma once
#include "stdafx.h"
#include "Loader.h"
#include "Fbo.h"
class PostProcessing
{
private:
	GLuint shader_fluid;
	GLint shader_fluid_loc;

	GLuint shader_hgauss;
	GLint shader_hgauss_loc;

	GLuint shader_vgauss;
	GLint shader_vgauss_loc;

	GLuint vao;
	void loadToVao(GLfloat* positions);
	void start();
	void stop();

	GLuint apply_fluid(GLuint colorTex, bool toFbo);
	GLuint apply_hgauss(GLuint colorTex, bool toFbo);
	GLuint apply_vgauss(GLuint colorTex, bool toFbo);

	Fbo fbo1;
	Fbo fbo2;
	Fbo fbo3;

	int curWidth;
	int curHeight;
public:
	void doPostProc(GLuint colorTex);
	void init(Loader loader);
	void resize(int width, int height);
	PostProcessing();
	~PostProcessing();
};

