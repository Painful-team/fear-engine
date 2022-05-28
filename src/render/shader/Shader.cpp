#include <glad/glad.h>
#include "Shader.hpp"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <memory>

#include <GLFW/glfw3.h>

namespace FearEngine::Render::Shaders
{
Shader::Shader()
 : shaderId(0)
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
	{}

	result.resize(size);
	in.seekg(0, std::ios::beg);
	in.read(result.data(), size);

	sources.insert({shaderType, result});
}

std::string& Shader::getSource(GLenum shaderType) { return sources[shaderType]; }

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

	GLint num_active_uniforms = 0;
	GLint num_active_blocks = 0;
	glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &num_active_uniforms);
	glGetProgramiv(program, GL_ACTIVE_UNIFORM_BLOCKS, &num_active_blocks);

	{
		constexpr uint32_t nameMaxSize = 128;
		char name[nameMaxSize];
		GLenum* uniform_types = (GLenum*)alloca(sizeof(GLenum) * num_active_uniforms);
		GLuint* uniform_indices = (GLuint*)alloca(sizeof(GLuint) * num_active_uniforms);
		for (int i = 0; i < num_active_uniforms; ++i)
		{
			GLsizei uniform_length = 0;
			GLint uniform_size = 0;
			GLenum uniform_type = 0;

			// grab uniform info
			glGetActiveUniform(program, (GLuint)i, nameMaxSize, &uniform_length, &uniform_size, &uniform_type, name);
			uniform_types[i] = uniform_type;
			uniform_indices[i] = glGetUniformBlockIndex(program, name);

			std::cout << name << ' ' << uniform_types[i] << ' ' << uniform_indices[i] << std::endl;
		}

		if (num_active_blocks > 0)
		{
			GLint* offsets = (GLint*)alloca(sizeof(GLint) * num_active_uniforms);
			glGetActiveUniformsiv(program, num_active_uniforms, uniform_indices, GL_UNIFORM_OFFSET, offsets);
		}

		//To prevent run-time check fail we have to create array of 3 GLint because of glGetActiveUniformBlockiv is writing more bytes that it has to
		GLint uniformIndexStride[3];
		char blockname[nameMaxSize];
		for (int i = 0; i < num_active_blocks; ++i)
		{
			GLsizei blockSize = 0;
			glGetActiveUniformBlockName(program, i, sizeof(blockname), &blockSize, blockname);
			uint32_t blockIndex = glGetUniformBlockIndex(program, blockname);

			glGetActiveUniformBlockiv(program, blockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, &*uniformIndexStride);
			std::cout << blockname << ' ' << blockSize << ' ' << uniformIndexStride[0] << std::endl;
		}
	}

	for (auto it : vec)
	{
		glDeleteShader(it);
	}

	shaderId = program;
}

void Shader::use() { glUseProgram(shaderId); }
uint32_t Shader::getId() { return shaderId; }
Shader::~Shader() { glDeleteProgram(shaderId); }
}  // namespace FearEngine::Render::Shaders
