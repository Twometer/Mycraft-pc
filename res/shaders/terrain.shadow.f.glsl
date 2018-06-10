#version 330 core

// Ouput data
in vec3 fragmentColor;
in vec2 UV;
out vec4 color;
in float visibility;
in vec4 shadowCoords;

uniform sampler2D texSampler;
uniform sampler2D shadowMap;
uniform vec3 skyColor;

const int pcfCount = 2;
const float totalTexels = (pcfCount * 2.0 + 1.0) * (pcfCount * 2.0 + 1.0);

void main()
{
	vec4 textureColor = texture(texSampler, UV);
	if(textureColor.a<0.1){
		discard;
	}
	
	float mapSize = 4096.0;
	float texelSize = 1.0 / mapSize;
	float total = 0.0;
	for(int x=-pcfCount; x <= pcfCount; x++) {
		for(int y=-pcfCount; y <= pcfCount; y++) {
			float objectNearestLight = texture(shadowMap, shadowCoords.xy + vec2(x,y) * texelSize).r;
			if(shadowCoords.z > objectNearestLight + 0.001) {
				total += 1.0;
			}
		}
	}
	
	total /= totalTexels;
	
	float lightFactor = 1.0f - (0.4 * total * shadowCoords.w);
	
	color = textureColor * vec4(fragmentColor, 1.0) * vec4(lightFactor,lightFactor,lightFactor,1.0);
	color = mix(vec4(0.72, 0.83, 0.996, 1.0), color, visibility);
}