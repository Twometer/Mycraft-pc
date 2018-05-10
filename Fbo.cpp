#include "Fbo.h"
#include "OpenGLRenderer.h"


Fbo::Fbo() {

}
Fbo::Fbo(int width, int height, int depthBufferType)
{
	this->width = width;
	this->height = height;
	initializeFrameBuffer(depthBufferType);
}

void Fbo::bindFrameBuffer() {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer);
	glViewport(0, 0, width, height);
}

void Fbo::unbindFrameBuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, OpenGLRenderer::width, OpenGLRenderer::height);
}

void Fbo::cleanUp() {
	glDeleteFramebuffers(1, &framebuffer);
	glDeleteTextures(1, &colorTexture);
	glDeleteTextures(1, &depthTexture);
	glDeleteRenderbuffers(1, &colorBuffer);
	glDeleteRenderbuffers(1, &depthBuffer);
}

GLint Fbo::getColorTexture() {
	return colorTexture;
}

GLint Fbo::getDepthTexture() {
	return depthTexture;
}

void Fbo::initializeFrameBuffer(int type) {
	createFrameBuffer();
	createTextureAttachment();
	if (type == DEPTH_RENDER_BUFFER) {
		createDepthBufferAttachment();
	}
	else if (type == DEPTH_TEXTURE) {
		createDepthTextureAttachment();
	}
	unbindFrameBuffer();
}

void Fbo::createFrameBuffer() {
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
}
void Fbo::createTextureAttachment() {
	glGenTextures(1, &colorTexture);
	glBindTexture(GL_TEXTURE_2D, colorTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTexture, 0);
}
void Fbo::createDepthTextureAttachment() {
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);
}
void Fbo::createDepthBufferAttachment() {
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);
}

Fbo::~Fbo()
{
}

