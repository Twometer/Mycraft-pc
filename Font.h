#pragma once
#include <string>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <map>

struct Character {
	GLuint     TextureID;  // ID handle of the glyph texture
	glm::ivec2 Size;       // Size of glyph
	glm::ivec2 Bearing;    // Offset from baseline to left/top of glyph
	GLuint     Advance;    // Offset to advance to next glyph
};


class Font
{
private:
	GLuint vbo;
	GLuint vao;
	std::map<GLchar, Character> Characters;
public:
	Font(const char* name, int size);
	~Font();
	glm::ivec2 getSize(std::string text);
	void renderText(std::string text, GLint loc, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
	void renderTextWithShadow(std::string text, GLint loc, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
};

