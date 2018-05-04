#version 140

in vec2 textureCoords;

out vec4 out_Colour;

uniform sampler2D colourTexture;

void main(void){

	vec4 colour = texture(colourTexture, textureCoords);
	out_Colour = colour;
	/*float brightness = (colour.r * 0.2126) + (colour.g * 0.7152) + (colour.b * 0.0722);
	if(brightness > 0.7){
		out_Colour = colour;
	}else{
		out_Colour = vec4(255,255,255,255);
	}*/
}