#version 460
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormals;
layout (location = 2) in vec3 aColor;

layout(std140, binding = 1) uniform Camera
{
	uniform mat4 projection;
	uniform mat4 model;
	uniform mat4 view;
};

uniform float wireframe;

out float frame;
out vec3 normal;
out vec3 color;

void main()
{
	frame = wireframe;
	normal = aNormals;

	color = aColor;

	gl_Position = projection * view * model * vec4(aPos, 1.0);
}
