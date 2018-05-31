#version 330 core

in vec2 textureCoords;
in vec4 fragmentColor;
out vec4 out_Colour;

uniform sampler2D colourTexture;

void main(void){
	out_Colour = fragmentColor * texture(colourTexture, textureCoords);
}