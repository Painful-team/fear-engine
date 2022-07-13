#version 460

#define MaxTextureSamples 32

layout(location = 1) out vec4 geometry;
layout(location = 2) out vec3 normals;
layout(location = 3) out vec4 albedo;
layout(location = 4) out vec3 ambient;
layout(location = 5) out int entityMap;

in vec4 pos;
in vec3 normal;
in vec2 otexCord;

layout(std140, binding = 2) uniform Material
{
	int specularTextureId;
	float shininess;

	vec3 ambientStrength;

	int diffuseTextureId;
};

layout(binding = 10) uniform sampler2D textures[MaxTextureSamples];

in flat int entityId;

void main()
{
	normals = normal;
	geometry = pos;

	ambient = ambientStrength;
	albedo = vec4(texture(textures[diffuseTextureId], otexCord).rgb, texture(textures[specularTextureId], otexCord).x);

	entityMap = entityId;
}
