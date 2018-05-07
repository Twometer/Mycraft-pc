#include "Skybox.h"
#include <string>

using namespace std;

GLuint textureId;
GLfloat boxSIZE = 500;

GLfloat VERTICES[] = {
	-boxSIZE,  boxSIZE, -boxSIZE,
	-boxSIZE, -boxSIZE, -boxSIZE,
	boxSIZE, -boxSIZE, -boxSIZE,
	boxSIZE, -boxSIZE, -boxSIZE,
	boxSIZE,  boxSIZE, -boxSIZE,
	-boxSIZE,  boxSIZE, -boxSIZE,

	-boxSIZE, -boxSIZE,  boxSIZE,
	-boxSIZE, -boxSIZE, -boxSIZE,
	-boxSIZE,  boxSIZE, -boxSIZE,
	-boxSIZE,  boxSIZE, -boxSIZE,
	-boxSIZE,  boxSIZE,  boxSIZE,
	-boxSIZE, -boxSIZE,  boxSIZE,

	boxSIZE, -boxSIZE, -boxSIZE,
	boxSIZE, -boxSIZE,  boxSIZE,
	boxSIZE,  boxSIZE,  boxSIZE,
	boxSIZE,  boxSIZE,  boxSIZE,
	boxSIZE,  boxSIZE, -boxSIZE,
	boxSIZE, -boxSIZE, -boxSIZE,

	-boxSIZE, -boxSIZE,  boxSIZE,
	-boxSIZE,  boxSIZE,  boxSIZE,
	boxSIZE,  boxSIZE,  boxSIZE,
	boxSIZE,  boxSIZE,  boxSIZE,
	boxSIZE, -boxSIZE,  boxSIZE,
	-boxSIZE, -boxSIZE,  boxSIZE,

	-boxSIZE,  boxSIZE, -boxSIZE,
	boxSIZE,  boxSIZE, -boxSIZE,
	boxSIZE,  boxSIZE,  boxSIZE,
	boxSIZE,  boxSIZE,  boxSIZE,
	-boxSIZE,  boxSIZE,  boxSIZE,
	-boxSIZE,  boxSIZE, -boxSIZE,

	-boxSIZE, -boxSIZE, -boxSIZE,
	-boxSIZE, -boxSIZE,  boxSIZE,
	boxSIZE, -boxSIZE, -boxSIZE,
	boxSIZE, -boxSIZE, -boxSIZE,
	-boxSIZE, -boxSIZE,  boxSIZE,
	boxSIZE, -boxSIZE,  boxSIZE
};

Skybox::Skybox()
{
}


Skybox::~Skybox()
{
}

void Skybox::initialize(Loader loader) {
	GLuint texId;
	glGenTextures(1, &texId);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texId);

	string textureFiles[6] = { "textures\\sky\\right.png", "textures\\sky\\left.png", "textures\\sky\\top.png", "textures\\sky\\bottom.png", "textures\\sky\\back.png", "textures\\sky\\front.png" };
	for (int i = 0; i < 6; i++) {
		IMAGE img = loader.loadImage(textureFiles[i].c_str());
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, img.width, img.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.data);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	textureId = texId;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 108, VERTICES, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void Skybox::render() {
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
}
