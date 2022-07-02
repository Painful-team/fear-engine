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
