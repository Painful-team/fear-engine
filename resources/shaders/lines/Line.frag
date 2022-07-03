#version 460

layout(location = 0) out vec4 FragColor;

in vec3 oColor;
void main()
{
	FragColor = vec4(oColor, 1.0f);
}
