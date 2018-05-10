#pragma once
#include <glad\glad.h>
#include <glm\common.hpp>
#include <string>
struct IMAGE {
	unsigned char* data;
	unsigned int dataLen, width, height;
	IMAGE(unsigned char* data, unsigned int dataLen, unsigned int width, unsigned int height) {
		this->data = data;
		this->dataLen = dataLen;
		this->width = width;
		this->height = height;
	}
	IMAGE() {

	}
};
class Loader
{
public:
	Loader();
	~Loader();
	IMAGE loadImage(const char* imagepath);
	GLuint loadTexture(const char * imagepath);
	GLuint loadShaders(const char * vertex_file_path, const char * fragment_file_path);
	GLuint loadShaders(const char * shaderName);
	static GLuint load2dVao(GLfloat * positions, int amount);
};

