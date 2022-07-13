#version 460
#include "../include/light.frag"

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 texCord;

out VS_OUT {
	vec2 texCord;
	vec3 viewPos;
} vOut;


layout(std140, binding = 1) uniform Camera1
{
	uniform mat4 projection;
	uniform mat4 view;
};

void main()
{
	gl_Position = vec4(pos, 1.0);

	vOut.texCord = texCord;

	vOut.viewPos = vec3(view[0][1], view[1][1], view[2][1]);
}
