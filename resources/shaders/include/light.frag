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
