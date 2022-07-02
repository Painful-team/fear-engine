#version 460

struct Light
{
	bool isPoint;

	vec3 pos;
	vec3 dir;

	float constant;
	float linear;
	float quadratic;

	float cutOff;
	float outerCutOff;

	vec3 lightColor;
};

struct DirLight
{
	vec3 dir;
	vec3 lightColor;
};

layout(location = 0) out vec4 FragColor;
layout(location = 1) out int entityMap;

uniform sampler2D textureId;

flat in int oEntityNum;

in vec3 normal;
in vec2 otexCord;
uniform vec3 color;
uniform bool disableTexture;

layout(std140, binding = 4) uniform DirLightBuffer
{
	DirLight dirLight;
};

void main()
{
	float diffuse = max(dot(dirLight.dir, normal), 0.0);

	FragColor = (diffuse + 0.1) *  mix(texture(textureId, otexCord), vec4(color, 1.0f), disableTexture);

	entityMap = oEntityNum;
}
