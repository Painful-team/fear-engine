#version 460

layout(std140, binding = 2) uniform Material
{
	int specularTextureId;
	float shininess;
	
	vec3 ambientStrength;

	int diffuseTextureId;
};

layout(binding = 10) uniform sampler2D textures[32];

out vec4 FragColor;
in float frame;
in vec3 normal;
in vec3 color;
in vec3 fragPos;
in vec3 viewPos;
in vec2 otexCord;

void main()
{
	if(frame == 0)
	{
		vec3 lightPos = vec3(5, 5, 5);
		vec3 lightColor = vec3(1, 1, 1);
		vec3 ambient = ambientStrength * lightColor * texture(textures[specularTextureId], otexCord).rgb;
		// diffuse 
		vec3 norm = normalize(normal);
		vec3 lightDir = normalize(lightPos - fragPos);
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = diff * lightColor * texture(textures[diffuseTextureId], otexCord).rgb;
		
		vec3 viewDir = normalize(viewPos - fragPos);
		vec3 reflectDir = reflect(-lightDir, norm);  
		vec3 spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess) * texture(textures[specularTextureId], otexCord).rgb;

		vec3 result = (ambient + diffuse + spec);
		FragColor = vec4(result, 1.0);
	}
	else
	{
		FragColor = vec4(0, 0, 0, 1.0f);
	}
}
