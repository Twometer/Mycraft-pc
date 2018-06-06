#pragma once
#include "stdafx.h"

class ShadowFrameBuffer
{
private:
	int width = 0;
	int height = 0;
	GLuint fbo = 0;
	GLuint shadowMap = 0;
	static void bind_frame_buffer(GLuint fb, int width, int height);
	static GLuint create_frame_buffer();
	static GLuint create_depth_buffer_attachment(int width, int height);
	void initialize_frame_buffer();
public:
	ShadowFrameBuffer(int width, int height);
	ShadowFrameBuffer();
	~ShadowFrameBuffer();
	void clean_up();
	void bind_frame_buffer();
	void unbind_frame_buffer();
	int get_shadow_map();
};

