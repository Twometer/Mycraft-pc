#pragma once
#include <glad\glad.h>
#include <glm\common.hpp>
#include <string>
class Loader
{
public:
	Loader();
	~Loader();
	GLuint loadPng(const char * imagepath);
	GLuint loadShaders(const char * vertex_file_path, const char * fragment_file_path);
	GLuint loadShaders(const char * shaderName);
	void loadFonts();
	void renderText(std::string text, GLint loc, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
};

