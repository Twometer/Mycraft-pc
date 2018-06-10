#pragma once
#include "stdafx.h"
#include "ShadowBox.h"
#include "ShadowFrameBuffer.h"
#include "Loader.h"

class ShadowMapRenderer
{
private:
	ShadowBox shadowBox;
	ShadowFrameBuffer shadowFbo;
	GLuint shadowShader;
	glm::mat4 projectionMatrix = glm::mat4(1.0f);
	glm::mat4 lightViewMatrix = glm::mat4(1.0f);
	glm::mat4 projectionViewMatrix = glm::mat4(1.0f);
	glm::mat4 offset;

	static const int SHADOW_MAP_SIZE = 2048 * 2;
	void prepare(glm::vec3 lightDirection);
	void finish();
	void update_light_view_matrix(glm::vec3 direction, glm::vec3 center);
	void update_ortho_projection_matrix(float width, float height, float length);
	static glm::mat4 create_offset();

	GLint shadowShaderLoc;
public:
	ShadowMapRenderer(Loader loader);
	~ShadowMapRenderer();
	void render();
	glm::mat4 get_to_shadow_map_space_matrix();
	void clean_up();
	int get_shadow_map();
	glm::mat4 get_light_space_transform();
};

