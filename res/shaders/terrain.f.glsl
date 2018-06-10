#version 330 core

// Ouput data
in vec3 fragmentColor;
in vec2 UV;
out vec4 color;
in float visibility;
in vec4 shadowCoords;

uniform sampler2D texSampler;
uniform vec3 skyColor;

void main()
{
	vec4 textureColor = texture(texSampler, UV);
	if(textureColor.a<0.1){
		discard;
	}
	
	color = textureColor * vec4(fragmentColor, 1.0);
	color = mix(vec4(0.72, 0.83, 0.996, 1.0), color, visibility);
}