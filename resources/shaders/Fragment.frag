#version 460
out vec4 FragColor;
in float frame;
in vec3 normal;
in vec3 color;

void main()
{
	if(frame == 0)
	{
		vec3 lightPos = vec3(5, 5, 5);
		vec3 lightColor = vec3(1, 1, 1);
		float ambientStrength = 0.1;
		vec3 ambient = ambientStrength * lightColor;
		// diffuse 
		vec3 norm = normalize(normal);
		vec3 lightDir = normalize(lightPos - color);
		float diff = max(dot(norm, lightDir), 0.0);
		vec3 diffuse = diff * lightColor;
            
		vec3 result = (ambient + diffuse) * vec3(1.0f, 0.5f, 0.31f);
		FragColor = vec4(result, 1.0);
	}
	else
	{
		FragColor = vec4(0, 0, 0, 1.0f);
	}
}
