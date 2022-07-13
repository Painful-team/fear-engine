#version 460
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormals;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 texCord;

layout(std140, binding = 1) uniform Camera1
{
	uniform mat4 projection;
	uniform mat4 view;
};

uniform mat4 model;

out vec4 pos;
out vec3 normal;
out vec2 otexCord;

void main()
{
	otexCord = texCord;

	//Todo think about moving that calculation to the CPU side, it could be efficient to calculate it only once
	normal = normalize(mat3(transpose(inverse(model))) * aNormals);

	vec4 obJpos = model * vec4(aPos, 1.0);
	gl_Position = projection * view * obJpos;

	pos = obJpos;
}
