#version 460
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormals;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 texCord;

out VS_OUT {
    vec3 normal;
} vs_out;

void main()
{
	vs_out.normal = aNormals;
	gl_Position = vec4(aPos, 1.0);
}
