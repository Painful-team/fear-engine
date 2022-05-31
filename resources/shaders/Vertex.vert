#version 460
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormals;
layout (location = 2) in vec3 aColor;

layout(std140, row_major, binding = 0) uniform Camera
{
uniform mat4 none;
uniform mat4 view;
};

uniform mat4 projection;
uniform mat4 model;

uniform float wireframe;

//uniform mat4 model;
//uniform mat4 view;
//uniform mat4 projection;


out float frame;
out vec3 normal;
out vec3 color;

void main()
{
	frame = wireframe;
	normal = aNormals;

	//bool a = projection1 == projection;

	color = aColor;

	gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0) ;
}
