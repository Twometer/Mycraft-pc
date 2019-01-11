#version 400

in vec3 position;

uniform mat4 projectionMatrix;
uniform mat4 modelviewMatrix;
uniform vec3 offset;
uniform float height;

void main(void){
	vec3 p = vec3(position.x, position.y * height, position.z);
	gl_Position = projectionMatrix * modelviewMatrix * vec4(p + offset, 1.0); 	
}