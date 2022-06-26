#version 460

layout (triangles) in;
layout (line_strip, max_vertices = 6) out;

layout(std140, binding = 1) uniform Camera
{
	uniform mat4 projection;
	uniform mat4 view;
	uniform mat4 model;
};


in VS_OUT {
    vec3 normal;
} gs_in [];

uniform float magnitude;

void GenerateLine(mat3 nMat, int index)
{
	gl_Position = projection * view * model * gl_in[index].gl_Position;
	EmitVertex();

	vec4 ptOnNormal = gl_in[index].gl_Position + vec4(normalize(nMat * gs_in[index].normal) * magnitude, 0.0f);
	gl_Position = projection * view * model * ptOnNormal;
	EmitVertex();
	EndPrimitive();
}

void main()
{
	mat3 nMat = mat3(transpose(inverse(model)));
    GenerateLine(nMat, 0);
    GenerateLine(nMat, 1);
    GenerateLine(nMat, 2);
}
