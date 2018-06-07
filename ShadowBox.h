#pragma once
#include <glm/glm.hpp>
class ShadowBox
{
	static const float offset;
	static const glm::vec4 up;
	static const glm::vec4 forward;
	static const float shadowDistance;

	float minX, maxX;
	float minY, maxY;
	float minZ, maxZ;

	glm::mat4 lightViewMatrix;

	float farHeight, farWidth, nearHeight, nearWidth;

	glm::vec4** calculate_frustum_vertices(glm::mat4 rotation, glm::vec3 forward, glm::vec3 center_near, glm::vec3 center_far);
	glm::vec4* calculate_light_space_frustum_corner(glm::vec3 startPoint, glm::vec3 direction, float width);
	glm::mat4 calculate_camera_rot_matrix();
	void calculate_widths_and_heights();
	float get_aspect_ratio();
public:
	ShadowBox();
	ShadowBox(glm::mat4 lightViewMatrix);
	~ShadowBox();
	void update();
	void set_light_view_matrix(glm::mat4 mat);
	glm::vec3 get_center();
	float get_width();
	float get_height();
	float get_length();
};

