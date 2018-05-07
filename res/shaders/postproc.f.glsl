#version 140

in vec2 textureCoords;

out vec4 out_Colour;

uniform sampler2D colourTexture;

uniform bool belowWater;

void main(void){

	vec4 colour = texture(colourTexture, textureCoords);
	if(belowWater){
		colour.r *= 0.5;
		colour.g *= 0.5;
	}
	out_Colour = colour;
	/*float brightness = (colour.r * 0.2126) + (colour.g * 0.7152) + (colour.b * 0.0722);
	if(brightness > 0.3){
		out_Colour = colour;
	}else{
		out_Colour = vec4(0,0,0,255);
	}*/
}