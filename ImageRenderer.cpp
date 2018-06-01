#include "ImageRenderer.h"



ImageRenderer::ImageRenderer()
{
}

ImageRenderer::ImageRenderer(int width, int height)
{
	fbo = new Fbo(width, height, NONE);
}

ImageRenderer::~ImageRenderer()
{
}

void ImageRenderer::render()
{
	if (fbo != nullptr) fbo->bindFrameBuffer();
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLE_STRIP,0,4);
	if (fbo != nullptr) fbo->unbindFrameBuffer();
}

int ImageRenderer::getTexture()
{
	return fbo->getColorTexture();
}