#include "Shader.hpp"
#include <glad/glad.h>

#include <algorithm>
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

	shaderId = program;
	initUniforms();

	for (auto it : vec)
	{
		glDeleteShader(it);
	}
}

void Shader::use() { glUseProgram(shaderId); }
uint32_t Shader::getId() { return shaderId; }
Shader::~Shader() { glDeleteProgram(shaderId); }
void Shader::initUniforms()
{
	GLint activeUniforms = 0;
	GLint activeBlocks = 0;
	glGetProgramiv(shaderId, GL_ACTIVE_UNIFORMS, &activeUniforms);
	glGetProgramiv(shaderId, GL_ACTIVE_UNIFORM_BLOCKS, &activeBlocks);

	constexpr uint16_t maxNameUniformBatch = 32;
	constexpr uint16_t nameMaxSize = 128;

	{
		// To prevent run-time check fail we have to create array of 3 GLint because of glGetActiveUniformBlockiv is writing more bytes that
		// it has to
		char blockname[nameMaxSize];
		for (int i = 0; i < activeBlocks; ++i)
		{
			GLsizei blockSize = 0;
			glGetActiveUniformBlockName(shaderId, i, sizeof(blockname), &blockSize, blockname);
			uint32_t blockIndex = glGetUniformBlockIndex(shaderId, blockname);

			GLint uniforms;
			glGetActiveUniformBlockiv(shaderId, blockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &uniforms);

			GLint* indices = (GLint*)alloca(sizeof(GLint*) * uniforms);
			glGetActiveUniformBlockiv(shaderId, blockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, &*indices);

			auto minMax = std::minmax_element(&indices[0], &indices[uniforms - 1]);

			BlockData blockData;
			blockData.blockIndex = blockIndex;
			blockData.uniformBeginIndex = *minMax.first;
			blockData.uniformEndIndex = *minMax.second;

			auto res = buffers.emplace(blockname, std::move(blockData));

			res.first->second.name = res.first->first;
		}

		for (auto& buf : buffers)
		{
			std::cout << buf.first << ' ' << buf.second.blockIndex << ' ' << buf.second.uniformBeginIndex << '-'
					  << buf.second.uniformEndIndex << std::endl;
		}

		char name[nameMaxSize];
		char** names = (char**)alloca(sizeof(char**) * (activeUniforms));
		GLuint* uniformIndices = (GLuint*)alloca(sizeof(GLuint) * activeUniforms);
		GLint* offsets = (GLint*)alloca(sizeof(GLint) * activeUniforms);
		for (uint16_t i = 0; i < activeUniforms; ++i)
		{
			GLsizei nameLength = 0;
			GLint uniform_size = 0;
			GLenum uniform_type = 0;

			glGetActiveUniform(shaderId, (GLuint)i, nameMaxSize, &nameLength, &uniform_size, &uniform_type, name);
			Uniform uniform(name, uniform_type);
			names[i] = uniform.name.data();

			/* for (auto& buf : buffers)
			{
				if (buf.second.uniformBeginIndex && buf.second.uniformEndIndex)
				{
				}
			}
			*/
		}

		if (activeBlocks > 0)
		{
			glGetUniformIndices(shaderId, activeUniforms, names, uniformIndices);
			glGetActiveUniformsiv(shaderId, activeUniforms, uniformIndices, GL_UNIFORM_OFFSET, offsets);
		}
	}
}

Shader::BlockData::BlockData(BlockData&& other) noexcept
 : name(other.name)
 , blockIndex(other.blockIndex)
 , uniformBeginIndex(other.uniformBeginIndex)
 , uniformEndIndex(other.uniformEndIndex)
 , uniforms(std::move(other.uniforms))
{}

Shader::BlockData& Shader::BlockData::operator=(BlockData&& other) noexcept
{
	name = other.name;

	blockIndex = other.blockIndex;

	uniformBeginIndex = other.uniformBeginIndex;
	uniformEndIndex = other.uniformEndIndex;

	uniforms = std::move(other.uniforms);

	return *this;
}
}  // namespace FearEngine::Render::Shaders
