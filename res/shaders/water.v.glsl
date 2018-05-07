#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexColor;
layout(location = 2) in vec2 vertexUV;

out vec3 fragmentColor;
out vec2 UV;

out float visibility;

uniform mat4 mvMatrix;
uniform mat4 prMatrix;

uniform int time;

const float density = 0.015;
const float gradient = 1.25;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main(){
	vec3 pos = vertexPosition_modelspace;
	pos.y += (sin(pos.x + time/400.0) + cos(pos.z + time/400.0)) / 10.0; //sin((rand(vec2(pos.x,pos.z)))/200.0f + time/400.0) / 10.0 + cos((pos.x * pos.z)/200.0f+ time/400.0) / 10.0;
	pos.y -= 0.2;
	vec4 positionRelativeToCam = mvMatrix * vec4(pos, 1.0);
    gl_Position = prMatrix * positionRelativeToCam;
	
	fragmentColor = vertexColor;
	UV = vertexUV;
	
	float distance = length(positionRelativeToCam.xyz);
	if(distance > 90){
		visibility = exp(-pow(((distance-90)*density),gradient));
		visibility = clamp(visibility, 0.0, 1.0);
	}else visibility = 1.0;
}

