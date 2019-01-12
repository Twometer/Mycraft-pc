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
	meta = new unsigned char[dataLen]();
	this->parent = parent;
	this->x = x;
	this->idx = idx;
	this->z = z;
}


Section::~Section()
{
	regularVbo = Vbo();
	transparentVbo = Vbo();
}

void Section::buildVertexData()
{
	if (state == STATE_DELETED || state == STATE_SHOULD_DELETE)
		return;
	delete builder;
	builder = new SectionBuilder(this);
	builder->build(x << 4, idx << 4, z << 4);
	this->vertexCount = builder->regularMesh->getVerticesCount();
	if(hasFluid)
		this->vertexCountX = builder->transparentMesh->getVerticesCount();
	state = STATE_SHOULD_UPLOAD;
}

void Section::uploadVertexData()
{
	if (state == STATE_DELETED || state == STATE_SHOULD_DELETE)
		return;

	regularVbo.destroy();
	transparentVbo.destroy();

	regularVbo.create(builder->regularMesh);

	if (hasFluid)
		transparentVbo.create(builder->transparentMesh);

	delete builder;
	builder = nullptr;

	/*int vertexCount = builder->regularMesh->getVerticesCount();
	int colorCount = builder->regularMesh->getColorsCount();
	int textureCount = builder->regularMesh->getColorsCount();

	if (vertexCount == 0)
		return;

	

	this->colorBuffer = colorbuffer;
	this->vertexBuffer = vertexbuffer;
	this->textureBuffer = texturebuffer;

	printf("Total V: %d, C: %d, T: %d\n", vertexCount, colorCount, textureCount);

	if (!hasFluid) {
		delete builder;
		builder = nullptr;
		return;
	}

	int vertexCountX = builder->transparentMesh->getVerticesCount();
	int colorCountX = builder->transparentMesh->getColorsCount();
	int textureCountX = builder->transparentMesh->getTexCoordsCount();

	GLuint vertexbufferX;
	glGenBuffers(1, &vertexbufferX);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbufferX);
	glBufferData(GL_ARRAY_BUFFER, vertexCountX * sizeof(GLfloat), builder->transparentMesh->vertices, GL_STATIC_DRAW);

	GLuint colorbufferX;
	glGenBuffers(1, &colorbufferX);
	glBindBuffer(GL_ARRAY_BUFFER, colorbufferX);
	glBufferData(GL_ARRAY_BUFFER, colorCountX * sizeof(GLfloat), builder->transparentMesh->colors, GL_STATIC_DRAW);

	GLuint texturebufferX;
	glGenBuffers(1, &texturebufferX);
	glBindBuffer(GL_ARRAY_BUFFER, texturebufferX);
	glBufferData(GL_ARRAY_BUFFER, textureCountX * sizeof(GLfloat), builder->transparentMesh->texCoords, GL_STATIC_DRAW);

	this->colorBufferX = colorbufferX;
	this->vertexBufferX = vertexbufferX;
	this->textureBufferX = texturebufferX;*/

}

void Section::destroy()
{
	state = State::STATE_SHOULD_DELETE;
}

void Section::check_deletion(bool transparencyPass)
{
	if (state == STATE_SHOULD_DELETE)
	{
		delete[] data;
		data = nullptr;
		if (transparencyPass)
		{
			/*glDeleteBuffers(1, &vertexBufferX);
			glDeleteBuffers(1, &colorBufferX);
			glDeleteBuffers(1, &textureBufferX);*/
			transparentVbo.destroy();
			state = STATE_DELETED;
			parent->destructionComplete(idx);
		}
		else
		{
			regularVbo.destroy();
			/*glDeleteBuffers(1, &vertexBuffer);
			glDeleteBuffers(1, &colorBuffer);
			glDeleteBuffers(1, &textureBuffer);*/
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
			/*
			if (vertexCountX == 0)
				return;
			glBindBuffer(GL_ARRAY_BUFFER, vertexBufferX);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glBindBuffer(GL_ARRAY_BUFFER, colorBufferX);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glBindBuffer(GL_ARRAY_BUFFER, textureBufferX);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glDrawArrays(GL_TRIANGLES, 0, vertexCountX / 3);*/
			transparentVbo.draw();
		}
		else
		{
			/*if (vertexCount == 0)
				return;
			glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
			
			glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

			glDrawArrays(GL_TRIANGLES, 0, vertexCount / 3);*/
			regularVbo.draw();
		}
	}
}

void Section::reload()
{
	if (state == STATE_SHOULD_RENDER)
	{
		state = STATE_SHOULD_BUILD;
		continueRender = true;
	}
}

void Section::setMeta(int x, int y, int z, unsigned char meta)
{
	*getBlockPointer(this->meta, x, y, z) = meta;
}

unsigned char Section::getMeta(int x, int y, int z)
{
	unsigned char* ptr = getBlockPointer(this->meta, x, y, z);
	if (ptr == nullptr) return 0;
	return *ptr;
}

void Section::setBlock(int x, int y, int z, unsigned char blockId, bool update)
{
	*getBlockPointer(data, x, y, z) = blockId;
	if (update) reload();
}

unsigned char Section::getBlock(int x, int y, int z)
{
	unsigned char* ptr = getBlockPointer(data, x, y, z);
	if (ptr == nullptr) return 0;
	return *ptr;
}

unsigned char* Section::getBlockPointer(unsigned char* data, int x, int y, int z)
{
	int idx = (y * 16 + z) * 16 + x;
	if (idx < 0 || idx > dataLen)
		return nullptr;
	return data + idx;
}
