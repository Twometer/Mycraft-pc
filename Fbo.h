#pragma once
#include "stdafx.h"

enum Type {
	NONE,
	DEPTH_TEXTURE,
	DEPTH_RENDER_BUFFER
};

class Fbo
{
	int width;
	int height;

	GLuint framebuffer;
	GLuint depthBuffer;
	GLuint colorBuffer;

	GLuint colorTexture;
	GLuint depthTexture;


public:
	Fbo();
	Fbo(int width, int height, int depthBufferType);
	~Fbo();
	void bindFrameBuffer();
	void unbindFrameBuffer();
	void cleanUp();
	GLint getColorTexture();
	GLint getDepthTexture();
private:
	void initializeFrameBuffer(int type);
	void createFrameBuffer();
	void createTextureAttachment();
	void createDepthTextureAttachment();
	void createDepthBufferAttachment();
};

