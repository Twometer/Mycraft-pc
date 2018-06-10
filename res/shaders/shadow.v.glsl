#version 330

layout(location = 0) in vec3 in_position;
layout(location = 2) in vec2 in_textureCoords;

out vec2 textureCoords;

uniform mat4 mvpMatrix;

void main(void){

	textureCoords = in_textureCoords;
	gl_Position = mvpMatrix * vec4(in_position, 1.0);
	
}