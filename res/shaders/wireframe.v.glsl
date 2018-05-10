#version 400

in vec3 position;

uniform mat4 projectionMatrix;
uniform mat4 modelviewMatrix;
uniform vec3 offset;

void main(void){
	gl_Position = projectionMatrix * modelviewMatrix * vec4(position + offset, 1.0); 	
}