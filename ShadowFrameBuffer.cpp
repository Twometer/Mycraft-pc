#include "ShadowFrameBuffer.h"
#include "OpenGLRenderer.h"


void ShadowFrameBuffer::bind_frame_buffer(GLuint fb, int width, int height)
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fb);
	glViewport(0, 0, width, height);
}

GLuint ShadowFrameBuffer::create_frame_buffer()
{
	GLuint frameBuffer;
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	return frameBuffer;
}

GLuint ShadowFrameBuffer::create_depth_buffer_attachment(int width, int height)
{
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture, 0);
	return texture;
}

void ShadowFrameBuffer::initialize_frame_buffer()
{
	fbo = create_frame_buffer();
	shadowMap = create_depth_buffer_attachment(width, height);
	unbind_frame_buffer();
}

ShadowFrameBuffer::ShadowFrameBuffer(int width, int height)
{
	this->width = width;
	this->height = height;
	initialize_frame_buffer();
}

ShadowFrameBuffer::ShadowFrameBuffer()
{
}

ShadowFrameBuffer::~ShadowFrameBuffer()
{
}

void ShadowFrameBuffer::clean_up()
{
	glDeleteFramebuffers(1, &fbo);
	glDeleteTextures(1, &shadowMap);
}

void ShadowFrameBuffer::bind_frame_buffer()
{
	bind_frame_buffer(fbo, width, height);
}

void ShadowFrameBuffer::unbind_frame_buffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, OpenGLRenderer::width, OpenGLRenderer::height);
}

int ShadowFrameBuffer::get_shadow_map()
{
	return shadowMap;
}