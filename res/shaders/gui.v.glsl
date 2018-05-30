#version 330 core
layout(location = 0) in vec2 position;
layout(location = 1) in vec4 vertexColor;
out vec4 fragmentColor;
uniform mat4 projection;

void main(void){
	gl_Position = projection * vec4(position, 0.0, 1.0);
	fragmentColor = vertexColor;
}