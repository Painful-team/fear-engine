#version 460
struct Light
{
	vec3 pos;
	bool isPoint;

	vec3 dir;

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


#define MaxDirLights 1000

layout(std140, binding = 5) uniform DirLightBuffer
{
	vec3 dir[MaxDirLights];
	vec3 lightColor[MaxDirLights];
	int dirLightCount;
};

float getLinear(float lightDist);
float getQuadratic(float lightDist);
vec3 calcLight(vec3 norm, vec3 viewDir, vec3 fragPos);
vec3 calcDirLight(vec3 norm, vec3 viewDir, vec3 fragPos);

layout(location = 0) out vec3 fragment;
layout(location = 7) out int entityMap;

uniform sampler2D geometry;
uniform sampler2D normals;
uniform sampler2D albedo;
uniform sampler2D ambients;

uniform samplerBuffer lights;
uniform int lightCount;

in VS_OUT {
	vec2 texCord;
	vec3 viewPos;
} fIn;

void main()
{
	vec4 fragPosD = texture(geometry, fIn.texCord);
	if(fragPosD.w != 1)
	{
		fragment = vec3(0.1);
		return;
	}

	vec3 norm = texture(normals, fIn.texCord).rgb;

	vec3 viewDir = normalize(fIn.viewPos - fragPosD.xyz);
	//
	vec3 result = vec3(0);
	result += calcLight(norm, viewDir, fragPosD.xyz);

	if(dirLightCount > 0)
	{
		result += calcDirLight(norm, viewDir, fragPosD.xyz);
	}

	fragment = result;

	entityMap = int(texelFetch(lights, 1).x);
}


#define elseifLessOrEqual(val, num, ret) else if(val <= num) { return ret; }

float getLinear(float lightDist)
{
// Special thanks to https://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation
	if(lightDist <= 7)
	{
		return 0.7;
	}
	elseifLessOrEqual(lightDist, 13, 0.35)
	elseifLessOrEqual(lightDist, 20, 0.22)
	elseifLessOrEqual(lightDist, 32, 0.14)
	elseifLessOrEqual(lightDist, 50, 0.09)
	elseifLessOrEqual(lightDist, 65, 0.07)
	elseifLessOrEqual(lightDist, 100, 0.045)
	elseifLessOrEqual(lightDist, 160, 0.027)
	elseifLessOrEqual(lightDist, 200, 0.022)
	elseifLessOrEqual(lightDist, 325, 0.014)
	elseifLessOrEqual(lightDist, 600, 0.007)
	elseifLessOrEqual(lightDist, 3250, 0.0014)

	return 0.0009;
};

float getQuadratic(float lightDist)
{
// Special thanks to https://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation
	if(lightDist <= 7)
	{
		return 1.8;
	}
	elseifLessOrEqual(lightDist, 13, 0.44)
	elseifLessOrEqual(lightDist, 20, 0.2)
	elseifLessOrEqual(lightDist, 32, 0.07)
	elseifLessOrEqual(lightDist, 50, 0.032)
	elseifLessOrEqual(lightDist, 65, 0.017)
	elseifLessOrEqual(lightDist, 100, 0.0075)
	elseifLessOrEqual(lightDist, 160, 0.0028)
	elseifLessOrEqual(lightDist, 200, 0.0019)
	elseifLessOrEqual(lightDist, 325, 0.0007)
	elseifLessOrEqual(lightDist, 600, 0.0002)
	elseifLessOrEqual(lightDist, 3250, 0.000007)

	return 0.000002;
};


Light unpackLight(int lightNum)
{
	Light light;

	const int lightSize = 13;
	vec4 temp = texelFetch(lights, lightNum);
	light.dir = vec3(0);
	light.isPoint = true;

	temp = texelFetch(lights, lightNum + 1);
	light.pos = vec3(0, 0, 5);
	light.linear = getLinear(25);

	light.quadratic = getQuadratic(25);

	temp = texelFetch(lights, lightNum + 2);
	light.lightColor = vec3(1);
	light.outerCutOff = temp.w;

	light.cutOff = texelFetch(lights, lightNum + 3).x;

	return light;
}

vec3 calcLight(vec3 norm, vec3 viewDir, vec3 fragPos)
{
	vec3 result = vec3(0);
	for(int i = 0; i < lightCount; ++i)
	{
		Light light = unpackLight(i);
		vec3 lightDir = normalize(light.pos - fragPos);

		// diffuse shading
		float diff = max(dot(norm, light.dir), 0.0);
		// specular shading
		vec3 reflectDir = reflect(-lightDir, norm);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
		// attenuation
		float distance = length(light.pos - fragPos);
		float attenuation = 1.0 / (1 + light.linear * distance + light.quadratic * (distance * distance));

		// combine results
		vec3 ambient = texture(ambients, fIn.texCord).rgb * texture(albedo, fIn.texCord).rgb;
		vec3 diffuse = diff * light.lightColor * texture(albedo, fIn.texCord).rgb;
		vec3 specular = spec * light.lightColor * texture(albedo, fIn.texCord).a;

		float intensity = 5;
		if(!light.isPoint)
		{
			// spotlight intensity
			float theta = dot(lightDir, normalize(-light.dir));
			float epsilon = light.cutOff - light.outerCutOff;
			intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
		}

		result += (diffuse + specular + ambient) * attenuation * intensity;
	}

	return result;
}


vec3 calcDirLight(vec3 norm, vec3 viewDir, vec3 fragPos)
{
	vec3 ambient = texture(ambients, fIn.texCord).rgb * lightColor[0] * texture(albedo, fIn.texCord).rgb;
	// diffuse
	vec3 lightDir = normalize(-dir[0]);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor[0] * texture(albedo, fIn.texCord).rgb;

	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32) * texture(albedo, fIn.texCord).a;

	return ambient + diffuse + spec;
}
