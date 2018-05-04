#version 140
in vec2 position;

out vec2 textureCoords;

//uniform int time;
//uniform boolean wave;

void main(void){

	gl_Position = vec4(position, 0.0, 1.0);
	textureCoords = position * 0.5 + 0.5;
	//if(wave)
//		textureCoords.x += sin(time/1000.0+position.y/2)/100.0;
}