#version 460
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormals;
layout (location = 2) in vec3 aColor;

//layout(std140, row_major) uniform Camera
//{
//	uniform mat4 model;
//	uniform mat4 view;
//	uniform mat4 projection;
//};
//
//layout(std140, row_major) uniform Data
//{
//	uniform float inframe;
//};


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float inframe;


out float frame;
out vec3 normal;
out vec3 color;

void main()
{
	frame = inframe;
	normal = aNormals;
	color = aColor;

	gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
