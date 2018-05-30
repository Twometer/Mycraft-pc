#version 330 core

in vec4 fragmentColor;
out vec4 out_Colour;

void main(void){
	out_Colour = fragmentColor;
}