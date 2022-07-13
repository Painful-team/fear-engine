#version 460
uniform vec3 color;

layout(location = 0) out vec3 FragColor;

void main()
{
	FragColor = color;
}
