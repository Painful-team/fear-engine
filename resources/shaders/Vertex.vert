#version 460
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexPos;
layout (location = 2) in vec3 aNormals;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float inframe;

out float frame;

void main()
{
	frame = inframe;
	gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
