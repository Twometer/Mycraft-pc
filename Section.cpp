#include "stdafx.h"
#include "Section.h"
#include <stdio.h>
#include "SectionBuilder.h"
#include "Chunk.h"
#include <vector>
#include "OpenGLRenderer.h"

using namespace std;


Section::Section(Chunk* parent, int x, int idx, int z)
{
	dataLen = 16 * 16 * 16;
	data = new unsigned char[dataLen]();
	this->parent = parent;
	this->x = x;
	this->idx = idx;
	this->z = z;
}


Section::~Section()
{
}

void Section::buildVertexData()
{
	if (builder != nullptr)
	{
		delete builder;
	}
	builder = new SectionBuilder(this);
	builder->build(x << 4, idx << 4, z << 4);
	this->vertexCount = builder->verticesAlloc;
	this->vertexCountX = builder->verticesAllocX;
	state = STATE_SHOULD_UPLOAD;
}

void Section::uploadVertexData()
{
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &colorBuffer);
	glDeleteBuffers(1, &textureBuffer);
	glDeleteBuffers(1, &vertexBufferX);
	glDeleteBuffers(1, &colorBufferX);
	glDeleteBuffers(1, &textureBufferX);

	int vertexCount = builder->verticesAlloc;
	int colorCount = builder->colorsAlloc;
	int textureCount = builder->textureCoordsAlloc;

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(GLfloat), builder->vertices, GL_STATIC_DRAW);

	GLuint colorbuffer;
	glGenBuffers(1, &colorbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
	glBufferData(GL_ARRAY_BUFFER, colorCount * sizeof(GLfloat), builder->colors, GL_STATIC_DRAW);

	GLuint texturebuffer;
	glGenBuffers(1, &texturebuffer);
	glBindBuffer(GL_ARRAY_BUFFER, texturebuffer);
	glBufferData(GL_ARRAY_BUFFER, textureCount * sizeof(GLfloat), builder->textureCoords, GL_STATIC_DRAW);

	this->colorBuffer = colorbuffer;
	this->vertexBuffer = vertexbuffer;
	this->textureBuffer = texturebuffer;

	int vertexCountX = builder->verticesAllocX;
	int colorCountX = builder->colorsAllocX;
	int textureCountX = builder->textureCoordsAllocX;

	GLuint vertexbufferX;
	glGenBuffers(1, &vertexbufferX);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbufferX);
	glBufferData(GL_ARRAY_BUFFER, vertexCountX * sizeof(GLfloat), builder->verticesX, GL_STATIC_DRAW);

	GLuint colorbufferX;
	glGenBuffers(1, &colorbufferX);
	glBindBuffer(GL_ARRAY_BUFFER, colorbufferX);
	glBufferData(GL_ARRAY_BUFFER, colorCountX * sizeof(GLfloat), builder->colorsX, GL_STATIC_DRAW);

	GLuint texturebufferX;
	glGenBuffers(1, &texturebufferX);
	glBindBuffer(GL_ARRAY_BUFFER, texturebufferX);
	glBufferData(GL_ARRAY_BUFFER, textureCountX * sizeof(GLfloat), builder->textureCoordsX, GL_STATIC_DRAW);

	this->colorBufferX = colorbufferX;
	this->vertexBufferX = vertexbufferX;
	this->textureBufferX = texturebufferX;
	delete builder;
	builder = nullptr;
}

void Section::destroy()
{
	state = State::STATE_SHOULD_DELETE;
}

void Section::check_deletion(bool transparencyPass)
{
	if (state == STATE_SHOULD_DELETE)
	{
		if (!dataCleared)
		{
			delete[] data;
			dataCleared = true;
		}
		if (transparencyPass)
		{
			glDeleteBuffers(1, &vertexBufferX);
			glDeleteBuffers(1, &colorBufferX);
			glDeleteBuffers(1, &textureBufferX);
			state = STATE_DELETED;
			parent->destructionComplete(idx);
		}
		else
		{
			glDeleteBuffers(1, &vertexBuffer);
			glDeleteBuffers(1, &colorBuffer);
			glDeleteBuffers(1, &textureBuffer);
		}
	}
}

int vertexDataBuiltAmount;

void Section::resetData()
{
	vertexDataBuiltAmount = 0;
}

void Section::render(bool transparencyPass, bool inFrustum)
{
	check_deletion(transparencyPass);
	if (state == STATE_SHOULD_BUILD)
	{
		state = STATE_AWAITING_BUILD;
		OpenGLRenderer::manager->schedule(this);
	}
	else if (state == STATE_SHOULD_UPLOAD && vertexDataBuiltAmount <= 8)
	{
		vertexDataBuiltAmount++;
		uploadVertexData();
		state = STATE_SHOULD_RENDER;
		continueRender = false;
	}
	if ((state == STATE_SHOULD_RENDER || continueRender) && inFrustum)
	{
		if (transparencyPass)
		{
			if (vertexCountX <= 0)
				return;
			glBindBuffer(GL_ARRAY_BUFFER, vertexBufferX);
			glVertexAttribPointer(
				0, // attribute 0. No particular reason for 0, but must match the layout in the shader.
				3, // size
				GL_FLOAT, // type
				GL_FALSE, // normalized?
				0, // stride
				(void*)0 // array buffer offset
			);
			glBindBuffer(GL_ARRAY_BUFFER, colorBufferX);
			glVertexAttribPointer(
				1, // attribute. No particular reason for 1, but must match the layout in the shader.
				3, // size
				GL_FLOAT, // type
				GL_FALSE, // normalized?
				0, // stride
				(void*)0 // array buffer offset
			);
			glBindBuffer(GL_ARRAY_BUFFER, textureBufferX);
			glVertexAttribPointer(
				2, // attribute. No particular reason for 1, but must match the layout in the shader.
				2, // size
				GL_FLOAT, // type
				GL_FALSE, // normalized?
				0, // stride
				(void*)0 // array buffer offset
			);
			glDrawArrays(GL_TRIANGLES, 0, vertexCountX / 3);
		}
		else
		{
			if (vertexCount <= 0)
				return;
			glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
			glVertexAttribPointer(
				0, // attribute 0. No particular reason for 0, but must match the layout in the shader.
				3, // size
				GL_FLOAT, // type
				GL_FALSE, // normalized?
				0, // stride
				(void*)0 // array buffer offset
			);
			glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
			glVertexAttribPointer(
				1, // attribute. No particular reason for 1, but must match the layout in the shader.
				3, // size
				GL_FLOAT, // type
				GL_FALSE, // normalized?
				0, // stride
				(void*)0 // array buffer offset
			);
			glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
			glVertexAttribPointer(
				2, // attribute. No particular reason for 1, but must match the layout in the shader.
				2, // size
				GL_FLOAT, // type
				GL_FALSE, // normalized?
				0, // stride
				(void*)0 // array buffer offset
			);
			glDrawArrays(GL_TRIANGLES, 0, vertexCount / 3);
		}
	}
}

void Section::reload()
{
	if (state != STATE_SHOULD_UPLOAD && state != STATE_AWAITING_BUILD)
	{
		state = STATE_SHOULD_BUILD;
		continueRender = true;
	}
}

void Section::generate()
{
	for (int x = 0; x < 16; x++)
	{
		for (int z = 0; z < 16; z++)
		{
			for (int y = 0; y < 16; y += 1)
			{
				setBlock(x, y, z, 1, false);
			}
		}
	}
}

void Section::setBlock(int x, int y, int z, unsigned char blockId, bool update)
{
	*getBlockPointer(x, y, z) = blockId;
	if (update) reload();
}

unsigned char Section::getBlock(int x, int y, int z)
{
	unsigned char* ptr = getBlockPointer(x, y, z);
	if (ptr == nullptr) return 0;
	return *ptr;
}

unsigned char* Section::getBlockPointer(int x, int y, int z)
{
	int idx = (y * 16 + z) * 16 + x;
	if (idx < 0 || idx > dataLen)
		return nullptr;
	return data + idx;
}
