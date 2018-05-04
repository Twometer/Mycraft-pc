#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Frustum
{
public:
	Frustum();
	~Frustum();
	void extractFrustum(glm::mat4 clip);
	bool CubeInFrustum(float x, float y, float z, float size);
};

