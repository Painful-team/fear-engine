#version 460
uniform vec3 color;

layout(location = 0) out vec4 FragColor;
layout(location = 1) out vec4 map;

void main()
{
	FragColor = vec4(color, 1.0f);
	map = map;
}
