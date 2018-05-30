#pragma once
#include <vector>
#include <glad/glad.h>
struct COLORDATA
{
	int r;
	int g;
	int b;
	int a;
	COLORDATA()
	{

	}

	COLORDATA(int r, int g, int b, int a)
	{
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}
};
class VboBuilder
{
private:
	GLuint vao;
	int dimen;
	std::vector<GLfloat> vertices;
	std::vector<GLfloat> colors;
	std::vector<GLfloat> texVertices;
	bool hasColor;
	bool hasTexture;
public:
	VboBuilder(int dimen);
	~VboBuilder();
	void vertex3(int x, int y, int z);
	void vertex2(int x, int y);
	void texture2(int u, int v);
	void drawRect(int x, int y, int width, int height, COLORDATA color, bool useTextures);
	void drawRect(int x, int y, int width, int height, COLORDATA color);
	void color(int r, int g, int b, int a);
	void build();
	void render();
	void buildAndRender();
};