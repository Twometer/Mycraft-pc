#pragma once
#include <glm/common.hpp>
class AABB
{
public:
	glm::vec3 p0;
	glm::vec3 p1;
	AABB(glm::vec3 p0, glm::vec3 p1);
	AABB expand(double x, double y, double z);
	AABB grow(double x, double y, double z);
	double clipXCollide(AABB c, double xa);
	double clipYCollide(AABB c, double ya);
	double clipZCollide(AABB c, double za);
	bool intersects(AABB c);
	void move(double x, double y, double z);
	~AABB();
};

