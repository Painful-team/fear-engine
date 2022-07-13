#version 460
#include "../include/light.frag"

layout(location = 0) out vec3 FragColor;
layout(location = 7) out int entityMap;

uniform sampler2D textureId;

in flat int oEntityNum;

in vec3 normal;
in vec2 otexCord;
uniform vec3 color;
uniform bool disableTexture;

layout(std140, binding = 4) uniform DirLightBuffer
{
	DirLight dirLight;
	int dirLightCount;
};

void main()
{
	float diffuse = max(dot(dirLight.dir, normal), 0.0);

	FragColor = ((diffuse + 0.1) *  mix(texture(textureId, otexCord), vec4(color, 1.0f), disableTexture)).rgb;

	entityMap = oEntityNum;
}
