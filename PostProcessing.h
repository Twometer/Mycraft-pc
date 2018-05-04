#pragma once
#include "stdafx.h"
class PostProcessing
{
private:
	GLuint shader;
	GLuint vao;
	void loadToVao(GLfloat* positions);
	void start();
	void stop();
public:
	void doPostProc(GLuint colorTex);
	void init();
	PostProcessing(GLuint shader);
	~PostProcessing();
};

