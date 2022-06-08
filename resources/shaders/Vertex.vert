#version 460
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormals;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 texCord;

layout(std140, binding = 1) uniform Camera
{
	uniform mat4 projection;

	uniform mat4 view;

};

uniform mat4 model;

uniform float wireframe;

out float frame;
out vec3 normal;
out vec3 color;
out vec3 fragPos;
out vec3 viewPos;
out vec2 otexCord;

void main()
{
	frame = wireframe;

	otexCord = texCord;

	//Todo think about moving that calculation to the CPU side, it could be efficient to calculate it only once
	normal = mat3(transpose(inverse(model))) * aNormals;

	color = aColor;

	viewPos = vec3(view[0][1], view[1][1], view[2][1]);
	fragPos = vec3(model * vec4(aPos, 1.0));
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}
