#include "Shader.hpp"

#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace FearEngine::Render::Shaders
{
	Shader::Shader():
		shaderId(0)
	{}

	void Shader::readShader(const char* path, GLenum shaderType)
{
	std::string result;
	std::ifstream in(path, std::ios::in);
	if (!in)
	{
		std::cerr << in.flags();
		return;
	}

	in.seekg(0, std::ios::end);
	size_t size = in.tellg();
	if (size != -1)
	{

	}

	result.resize(size);
	in.seekg(0, std::ios::beg);
	in.read(result.data(), size);

	sources.insert({ shaderType, result });
}

std::string& Shader::getSource(GLenum shaderType)
{
	return sources[shaderType];
}

void Shader::compile()
{
	uint32_t program = glCreateProgram();
	std::vector<uint32_t> vec(sources.size());
	for (const auto& it : sources)
	{
		uint32_t shader = glCreateShader(it.first);
		const char* shaderCode = it.second.c_str();
		glShaderSource(shader, 1, &shaderCode, NULL);
		glCompileShader(shader);

		int success;
		char infoLog[512];
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		vec.push_back(shader);

		glAttachShader(program, shader);
	}

	glLinkProgram(program);

	int success;
	char infoLog[512];
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	for (auto it : vec)
	{
		glDeleteShader(it);
	}

	shaderId = program;
}
void Shader::use()
{
	glUseProgram(shaderId);
}
uint32_t Shader::getId()
{
	return shaderId;
}
Shader::~Shader()
{
	glDeleteProgram(shaderId);
}
}
