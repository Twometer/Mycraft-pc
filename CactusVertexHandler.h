#pragma once
#include "VertexHandler.h"
class CactusVertexHandler 
	: public VertexHandler
{

	glm::vec3 process(int meta, GLfloat x, GLfloat y, GLfloat z, glm::vec3 worldPos, FACE face)
	{
		return moveIn(x, y, z, face, 0.0625f);
	}

};