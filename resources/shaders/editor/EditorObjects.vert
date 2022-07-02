#version 460
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormals;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 texCord;

layout (location = 4) in vec4 tranformRow1;
layout (location = 5) in vec4 tranformRow2;
layout (location = 6) in vec4 tranformRow3;
layout (location = 7) in vec4 tranformRow4;
layout (location = 8) in int entityNum;

layout(std140, binding = 1) uniform Camera
{
	uniform mat4 projection;
	uniform mat4 view;
};

out vec2 otexCord;
out vec3 normal;

flat out int oEntityNum;

void main()
{
	mat4 model = mat4(tranformRow1, tranformRow2, tranformRow3, tranformRow4);
	normal = mat3(transpose(inverse(model))) * aNormals;

	gl_Position = projection * view * model * vec4(aPos, 1.0);

	oEntityNum = entityNum;

	otexCord = texCord;
}
