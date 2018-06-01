#pragma once
#include "Fbo.h"
class ImageRenderer
{
private:
	Fbo* fbo = nullptr;
public:
	ImageRenderer(int width, int height);
	ImageRenderer();
	~ImageRenderer();
	void render();
	int getTexture();
};
