#version 140

in vec2 textureCoords;

out vec4 out_Colour;

uniform sampler2D colourTexture;

void main(void){
	vec4 colour = texture(colourTexture, textureCoords);
	out_Colour = colour;
}