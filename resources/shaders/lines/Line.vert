#version 460
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 color;

layout(std140, binding = 1) uniform Camera
{
	uniform mat4 projection;
	uniform mat4 view;
};

out vec3 oColor;
void main()
{
	oColor = color;
	gl_Position = projection * view * vec4(aPos, 1.0);
}
