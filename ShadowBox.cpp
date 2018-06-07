#include "ShadowBox.h"
#include "Controls.h"
#include "OpenGLRenderer.h"
#include "Settings.h"

using namespace glm;

const float ShadowBox::offset = 10;
const vec4 ShadowBox::up = vec4(0, 1, 0, 0);
const vec4 ShadowBox::forward = vec4(0, 0, -1, 0);
const float ShadowBox::shadowDistance = 100;


vec4** ShadowBox::calculate_frustum_vertices(mat4 rotation, vec3 forward, vec3 center_near, vec3 center_far)
{
	vec3 upVec = rotation * up;
	vec3 rightVec = cross(forward, upVec);
	vec3 downVec = -upVec;
	vec3 leftVec = -rightVec;
	vec3 farTop = center_far + (upVec * farHeight);
	vec3 farBottom = center_far + (downVec * farHeight);
	vec3 nearTop = center_near + (upVec * nearHeight);
	vec3 nearBottom = center_near + (downVec * nearHeight);
	vec4**  points = new vec4*[8];
	points[0] = calculate_light_space_frustum_corner(farTop, rightVec, farWidth);
	points[1] = calculate_light_space_frustum_corner(farTop, leftVec, farWidth);
	points[2] = calculate_light_space_frustum_corner(farBottom, rightVec, farWidth);
	points[3] = calculate_light_space_frustum_corner(farBottom, leftVec, farWidth);
	points[4] = calculate_light_space_frustum_corner(nearTop, rightVec, nearWidth);
	points[5] = calculate_light_space_frustum_corner(nearTop, leftVec, nearWidth);
	points[6] = calculate_light_space_frustum_corner(nearBottom, rightVec, nearWidth);
	points[7] = calculate_light_space_frustum_corner(nearBottom, leftVec, nearWidth);
	return points;
}

vec4* ShadowBox::calculate_light_space_frustum_corner(vec3 startPoint, vec3 direction, float width)
{
	vec3 point = startPoint + (direction * width);
	vec4 point4 = lightViewMatrix * vec4(point, 1.0f);
	return new vec4(point4);
}

mat4 ShadowBox::calculate_camera_rot_matrix()
{
	mat4 rotation = mat4(1.0f);
	vec2 camRotation = OpenGLRenderer::controls->getRotation();
	rotation = rotate(rotation, radians(-camRotation.x), vec3(0, 1, 0));
	rotate(rotation, radians(-camRotation.y), vec3(1, 0, 0));
	return rotation;
}

void ShadowBox::calculate_widths_and_heights()
{
	farWidth = shadowDistance * tan(radians(Settings::FOV));
	nearWidth = Controls::near_plane * tan(radians(Settings::FOV));
	farHeight = farWidth / get_aspect_ratio();
	nearHeight = nearWidth / get_aspect_ratio();
}

float ShadowBox::get_aspect_ratio()
{
	return (float)OpenGLRenderer::width / (float)OpenGLRenderer::height;
}

ShadowBox::ShadowBox()
{
}

ShadowBox::ShadowBox(mat4 lightViewMatrix)
{
	this->lightViewMatrix = lightViewMatrix;
	calculate_widths_and_heights();
}


ShadowBox::~ShadowBox()
{
}

void ShadowBox::update()
{
	vec3 camera_pos = OpenGLRenderer::controls->getEyePosition();

	mat4 rotation = calculate_camera_rot_matrix();
	vec3 forwardVec = rotation * forward;
	vec3 toFar = forwardVec * shadowDistance;
	vec3 toNear = forwardVec * Controls::near_plane;
	vec3 centerNear = toNear + camera_pos;
	vec3 centerFar = toFar + camera_pos;

	vec4** points = calculate_frustum_vertices(rotation, forwardVec, centerNear, centerFar);
	bool first = true;
	for (int i = 0; i < 8; i++)
	{
		vec4 point = *points[i];
		if (first)
		{
			minX = point.x;
			maxX = point.x;
			minY = point.y;
			maxY = point.y;
			minZ = point.z;
			maxZ = point.z;
			first = false;
			continue;
		}
		if (point.x > maxX) maxX = point.x;
		else if (point.x < minX) minX = point.x;
		if (point.y > maxY) maxY = point.y;
		else if (point.y < minY) minY = point.y;
		if (point.z > maxZ) maxZ = point.z;
		else if (point.z < minZ) minZ = point.z;
	}
	delete[] points;
	maxZ += offset;
}

void ShadowBox::set_light_view_matrix(glm::mat4 mat)
{
	lightViewMatrix = mat;
}

vec3 ShadowBox::get_center()
{
	float x = (minX + maxX) / 2.0f;
	float y = (minY + maxY) / 2.0f;
	float z = (minZ + maxZ) / 2.0f;
	vec4 cen = vec4(x, y, z, 1);
	mat4 inverted_light = inverse(lightViewMatrix);
	return inverted_light * cen;
}

float ShadowBox::get_width()
{
	return maxX - minX;
}

float ShadowBox::get_height()
{
	return maxY - minY;
}

float ShadowBox::get_length()
{
	return maxZ - minZ;
}
