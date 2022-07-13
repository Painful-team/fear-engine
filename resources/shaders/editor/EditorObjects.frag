#version 460

layout(location = 0) out vec3 FragColor;
layout(location = 5) out int entityMap;

uniform sampler2D textureId;

in flat int oEntityNum;

in vec3 normal;
in vec2 otexCord;
uniform vec3 color;
uniform bool disableTexture;

void main()
{
	vec3 result = vec3(0);

	vec3 textureColor = mix(texture(textureId, otexCord), vec4(color, 1.0f), disableTexture).rgb;

	vec3 lightDir = normalize(glm::vec3(-1.8, -1.8, -1));
	float diffuse = max(dot(normal, lightDir), 0.0);

	FragColor = (diffuse + 0.3) * textureColor;
	entityMap = oEntityNum;
}
