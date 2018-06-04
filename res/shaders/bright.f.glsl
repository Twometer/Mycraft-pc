#version 140

in vec2 textureCoords;
out vec4 out_Colour;
uniform sampler2D colourTexture;

void main(void){
	vec4 colour = texture(colourTexture, textureCoords);
	float brightness = (colour.r * 0.2126) + (colour.g * 0.7152) + (colour.b * 0.0722);
	if(brightness > 0.8){
		out_Colour = colour;
	}else{
		out_Colour = vec4(0.0);
	}
	out_Colour.a = 1.0;
}