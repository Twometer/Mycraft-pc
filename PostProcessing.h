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

	GLuint shader_bright;
	GLuint shader_mix;

	GLuint vao;
	void loadToVao(GLfloat* positions);
	void start();
	void stop();

	void apply_fluid(GLuint colorTex, Fbo* fbo);
	void apply_hgauss(GLuint colorTex, Fbo* fbo, float intensity);
	void apply_vgauss(GLuint colorTex, Fbo* fbo, float intensity);
	void apply_bright(GLuint colorTex, Fbo* fbo);
	void apply_mix(GLuint colorTex, GLuint colorTex2 , Fbo* fbo);

	Fbo fbo1;
	Fbo fbo2;
	Fbo fbo3;

	Fbo fboDS1;
	Fbo fboDS2;

	int curWidth;
	int curHeight;
public:
	void doPostProc(GLuint colorTex);
	void init(Loader loader);
	void resize(int width, int height);
	PostProcessing();
	~PostProcessing();
};

