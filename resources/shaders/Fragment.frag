#version 460

//Todo Remove that constant and think about light batching.
#define MAX_LIGHTS 100

layout(std140, binding = 2) uniform Material
{
	int specularTextureId;
	float shininess;
	
	vec3 ambientStrength;

	int diffuseTextureId;
};

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

layout(std140, binding = 3) uniform LightBuffer
{
	uniform Light lights[MAX_LIGHTS];
};

layout(std140, binding = 4) uniform DirLightBuffer
{
	DirLight dirLight;
};

layout(binding = 10) uniform sampler2D textures[32];

out vec4 FragColor;
in float frame;
in vec3 normal;
in vec3 color;
in vec3 fragPos;
in vec3 viewPos;
in vec2 otexCord;

vec3 calcLight(Light light, vec3 norm, vec3 viewDir);
vec3 calcDirLight(vec3 norm, vec3 viewDir);

void main()
{
	if(frame == 0)
	{
		vec3 result = vec3(0);
		vec3 norm = normalize(normal);
		vec3 viewDir = normalize(viewPos - fragPos);

		result += calcDirLight(norm, viewDir);

		//for(int i = 0; i < MAX_LIGHTS; ++i)
		//{
		//	result += calcLight(lights[i], norm, viewDir);
		//}

		FragColor = vec4(result, 1);
	}
	else
	{
		FragColor = vec4(0, 0, 0, 1.0f);
	}
}

vec3 calcLight(Light light, vec3 norm, vec3 viewDir)
{

	vec3 lightDir = normalize(light.pos - fragPos);

	// diffuse shading
	float diff = max(dot(normal, light.dir), 0.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	// attenuation
	float distance = length(light.pos - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
	// spotlight intensity
	float theta = dot(lightDir, normalize(-light.dir)); 
	float epsilon = light.cutOff - light.outerCutOff;
	// combine results
	vec3 ambient = ambientStrength * texture(textures[specularTextureId], otexCord).rgb;
	vec3 diffuse = diff * light.lightColor * texture(textures[specularTextureId], otexCord).rgb;
	vec3 specular = spec * light.lightColor * texture(textures[specularTextureId], otexCord).rgb;

	float intensity = 1;
	if(!light.isPoint)
	{
		intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
	}

	return (diffuse + specular + ambient) * attenuation * intensity;
}

vec3 calcDirLight(vec3 norm, vec3 viewDir)
{
		vec3 lightPos = vec3(5, 5, 5);
		vec3 ambient = ambientStrength * dirLight.lightColor * texture(textures[diffuseTextureId], otexCord).rgb;
		// diffuse 
		vec3 lightDir = normalize(-dirLight.dir);
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = diff * dirLight.lightColor * texture(textures[diffuseTextureId], otexCord).rgb;
		
		vec3 reflectDir = reflect(-lightDir, norm);  
		vec3 spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess) * texture(textures[specularTextureId], otexCord).rgb;

	return ambient + diffuse + spec;
}
