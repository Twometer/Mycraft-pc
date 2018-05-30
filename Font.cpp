#include "Font.h"
#include <iostream>
#include "OpenGLRenderer.h"

using namespace std;
using namespace glm;

void loadColor(GLint loc, int x, int y, int z) {
	glUniform3f(loc, x / (double)255, y / (double)255, z / (double)255);
}

string stripColorCodes(string str) {
	string out = string();
	bool lastCc = false;
	for (int i = 0; i < str.length(); i++) {
		char c = str.at(i);
		if (lastCc) {
			lastCc = false;
			continue;
		}
		if (c == '§') {
			lastCc = true;
			continue;
		}
		out += c;
	}
	return out;
}

Font::Font(const char* name, int size)
{
	FT_Library ft;
	if (FT_Init_FreeType(&ft))
		cout << "ERROR::FREETYPE: Could not init FreeType Library" << endl;

	FT_Face face;
	if (FT_New_Face(ft, "fonts/Roboto.ttf", 0, &face))
		cout << "ERROR::FREETYPE: Failed to load font" << endl;
	FT_Set_Pixel_Sizes(face, 0, 15);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

	for (GLubyte c = 0; c < 128; c++)
	{
		// Load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			cout << "ERROR::FREETYTPE: Failed to load Glyph" << endl;
			continue;
		}
		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		Characters.insert(pair<GLchar, Character>(c, character));
	}

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


Font::~Font()
{
}

void Font::renderTextWithShadow(string text, GLint loc, GLfloat x, GLfloat y, GLfloat scale, vec3 color) {
	renderText(stripColorCodes(text), loc, x, y, scale, vec3(0.0, 0.0, 0.0));
	renderText(text, loc, x - 1.0f, y - 1.0f, scale, color);
}

glm::ivec2 Font::getSize(string text) {
	int w = 0;
	int h = 0;
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		if (*c == '§') continue;
		if (c != text.begin() && *(c - 1) == '§') continue;

		Character ch = Characters[*c];
		w += ch.Size.x + ch.Bearing.x;
		if (ch.Size.y > h)
			h = ch.Size.y;
	}
	return ivec2(w, h);
}

void Font::renderText(string text, GLint loc, GLfloat x, GLfloat y, GLfloat scale, vec3 color) {
	y = OpenGLRenderer::height - y;
	glUniform3f(loc, color.x, color.y, color.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(vao);
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		if (*c == '§') continue;
		if (c != text.begin() && *(c - 1) == '§') {
			char code = *c;
			if (code == '0') loadColor(loc, 0, 0, 0);
			else if (code == '1') loadColor(loc, 0, 0, 170);
			else if (code == '2') loadColor(loc, 0, 170, 0);
			else if (code == '3') loadColor(loc, 0, 170, 170);
			else if (code == '4') loadColor(loc, 170, 0, 0);
			else if (code == '5') loadColor(loc, 170, 0, 170);
			else if (code == '6') loadColor(loc, 255, 170, 0);
			else if (code == '7') loadColor(loc, 170, 170, 170);
			else if (code == '8') loadColor(loc, 85, 85, 85);
			else if (code == '9') loadColor(loc, 85, 85, 255);
			else if (code == 'a') loadColor(loc, 85, 255, 85);
			else if (code == 'b') loadColor(loc, 85, 255, 255);
			else if (code == 'c') loadColor(loc, 255, 85, 85);
			else if (code == 'd') loadColor(loc, 255, 85, 255);
			else if (code == 'e') loadColor(loc, 255, 255, 85);
			else if (code == 'f') loadColor(loc, 255, 255, 255);
			continue;
		}

		Character ch = Characters[*c];

		GLfloat xpos = x + ch.Bearing.x * scale;
		GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * scale;
		// Update VBO for each character
		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },
		{ xpos,     ypos,       0.0, 1.0 },
		{ xpos + w, ypos,       1.0, 1.0 },

		{ xpos,     ypos + h,   0.0, 0.0 },
		{ xpos + w, ypos,       1.0, 1.0 },
		{ xpos + w, ypos + h,   1.0, 0.0 }
		};
		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
