#include "ShadowMapRenderer.h"
#include "OpenGLRenderer.h"

using namespace glm;

void ShadowMapRenderer::prepare(vec3 lightDirection)
{
	update_ortho_projection_matrix(shadowBox.get_width(), shadowBox.get_height(), shadowBox.get_length());
	update_light_view_matrix(lightDirection, shadowBox.get_center());
	projectionViewMatrix = projectionMatrix * lightViewMatrix;
	shadowFbo.bind_frame_buffer();
	glEnable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(shadowShader);
	mat4 mvpMatrix = projectionViewMatrix * mat4(1.0f);
	glUniformMatrix4fv(shadowShaderLoc, 1, false, &mvpMatrix[0][0]);
}

void ShadowMapRenderer::finish()
{
	glUseProgram(0);
	shadowFbo.unbind_frame_buffer();
}

void ShadowMapRenderer::update_light_view_matrix(vec3 direction, vec3 center)
{
	direction = normalize(direction);
	center *= -1;
	lightViewMatrix = mat4(1.0f); // Create identity matrix
	float pitch = acos(length(vec2(direction.x, direction.z)));
	rotate(lightViewMatrix, radians(pitch), vec3(1, 0, 0));
	float yaw = degrees(atan((float)direction.x / direction.z));
	yaw = direction.z > 0 ? yaw - 180 : yaw;
	rotate(lightViewMatrix, -radians(yaw), vec3(0, 1, 0));
	translate(lightViewMatrix, center);
}

void ShadowMapRenderer::update_ortho_projection_matrix(float width, float height, float length)
{
	projectionMatrix = mat4(1.0f); // Set to identity matrix
	projectionMatrix[0][0] = 2.0f / width;
	projectionMatrix[1][1] = 2.0f / height;
	projectionMatrix[2][2] = -2.0f / length;
	projectionMatrix[3][3] = 1;
}

mat4 ShadowMapRenderer::create_offset()
{
	mat4 offset = mat4(1.0f);
	translate(offset, vec3(0.5f, 0.5f, 0.5f));
	scale(offset, vec3(0.5f, 0.5f, 0.5f));
	return offset;
}

ShadowMapRenderer::ShadowMapRenderer(Loader loader)
{
	shadowBox = ShadowBox(lightViewMatrix);
	shadowFbo = ShadowFrameBuffer(SHADOW_MAP_SIZE, SHADOW_MAP_SIZE);
	shadowShader = loader.loadShaders("shadow");
	shadowShaderLoc = glGetUniformLocation(shadowShader, "mvpMatrix");
	offset = create_offset();
}


ShadowMapRenderer::~ShadowMapRenderer()
{
}

void ShadowMapRenderer::render()
{
	shadowBox.update();
	vec3 sunPosition = vec3(1000000, 1500000, -1000000);
	vec3 lightDirection = -sunPosition;
	prepare(lightDirection);
	OpenGLRenderer::world->render(false);
	finish();
}

mat4 ShadowMapRenderer::get_to_shadow_map_space_matrix()
{
	return offset * projectionViewMatrix;
}

void ShadowMapRenderer::clean_up()
{
	shadowFbo.clean_up();
}

int ShadowMapRenderer::get_shadow_map()
{
	return shadowFbo.get_shadow_map();
}

mat4 ShadowMapRenderer::get_light_space_transform()
{
	return lightViewMatrix;
}
