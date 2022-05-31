#include "Shader.hpp"
#include <glad/glad.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include <GLFW/glfw3.h>

#include <core/Engine.hpp>

namespace FearEngine::Render::Shaders
{
Shader::Shader()
 : shaderId(0)
{
	BlockData blockData;
	blockData.blockIndex = -1;
	blockData.uniformBeginIndex = -1;
	blockData.uniformEndIndex = -1;

	auto res = buffers.emplace(baseBufferName, std::move(blockData));

	res.first->second.name = res.first->first;
}

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

Uniform& Shader::findUniform(const std::string& name)
{
	for (auto& buf : buffers)
	{
		auto& it = buf.second.uniforms.find(name);
		if (it != buf.second.uniforms.end())
		{
			return it->second;
		}
	}

	return Uniform{};
}

Shader::UniformStorage& Shader::findBuffer(const std::string& name)
{
	auto& it = buffers.find(name);
	if (it == buffers.end())
	{
		return Shader::UniformStorage{};
	}

	return buffers.at(name).uniforms;
}

void Shader::updateBuffers()
{
	for (auto& buf: buffers)
	{
		if (buf.first == baseBufferName)
		{
			continue;
		}

		glBindBuffer(GL_UNIFORM_BUFFER, buf.second.bufferBlockId);
		for (auto un : buf.second.uniforms)
		{
			glBufferSubData(GL_UNIFORM_BUFFER, un.second.offset, sizeof(glm::mat4), buf.second.bufferMemory + un.second.offset);
		}
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		//glBindBuffer(GL_UNIFORM_BUFFER, buf.second.bufferBlockId);
		//glBufferData(GL_UNIFORM_BUFFER, buf.second.blockSize, buf.second.bufferMemory, GL_DYNAMIC_DRAW);
		//glBindBufferRange(GL_UNIFORM_BUFFER, buf.second.binding, buf.second.bufferBlockId, 0, buf.second.blockSize);
	}
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

	shaderId = program;
	initUniforms();

	for (auto it : vec)
	{
		glDeleteShader(it);
	}
}

void Shader::use() 
{ 
	glUseProgram(shaderId); 
}

uint32_t Shader::getId() { return shaderId; }
Shader::~Shader() { glDeleteProgram(shaderId); }

//TODO FIX PROBLEM WITH MULTIPLE BINDINGS OF TWO SHADERS
void Shader::initUniforms()
{
	GLint activeUniforms = 0;
	GLint activeBlocks = 0;
	glGetProgramiv(shaderId, GL_ACTIVE_UNIFORMS, &activeUniforms);
	glGetProgramiv(shaderId, GL_ACTIVE_UNIFORM_BLOCKS, &activeBlocks);

	constexpr uint16_t maxNameUniformBatch = 32;
	constexpr uint16_t nameMaxSize = 128;

	{
		uint8_t* busyBinding = (uint8_t*)alloca(sizeof(uint8_t) * Engine::getRender()->graphicsData[GL_MAX_UNIFORM_BUFFER_BINDINGS] - 1);
		char blockname[nameMaxSize];
		for (int i = 0; i < activeBlocks; ++i)
		{
			GLsizei blockNameLength = 0;
			glGetActiveUniformBlockName(shaderId, i, nameMaxSize, &blockNameLength, blockname);
			uint32_t blockIndex = glGetUniformBlockIndex(shaderId, blockname);

			GLint uniforms;
			glGetActiveUniformBlockiv(shaderId, blockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &uniforms);
 
			GLint blockSize;
			glGetActiveUniformBlockiv(shaderId, blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);

			GLint binding;
			glGetActiveUniformBlockiv(shaderId, blockIndex, GL_UNIFORM_BLOCK_BINDING, &binding);

			glUniformBlockBinding(shaderId, blockIndex, binding);

			GLint* indices = (GLint*)alloca(sizeof(GLint*) * uniforms);
			glGetActiveUniformBlockiv(shaderId, blockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, &*indices);

			auto minMax = std::minmax_element(&indices[0], &indices[uniforms]);

			BlockData blockData;
			blockData.blockIndex = blockIndex;
			blockData.uniformBeginIndex = *minMax.first;
			blockData.uniformEndIndex = *minMax.second;
			blockData.blockSize = blockSize;
			blockData.binding = binding;

			blockData.bufferMemory = new int8_t[blockData.blockSize];

			glGenBuffers(1, &blockData.bufferBlockId);
			
			glBindBuffer(GL_UNIFORM_BUFFER, blockData.bufferBlockId);
			glBufferData(GL_UNIFORM_BUFFER, blockData.blockSize, 0, GL_STREAM_DRAW);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);


			glBindBufferRange(GL_UNIFORM_BUFFER, blockData.binding, blockData.bufferBlockId, 0, blockData.blockSize);
			auto res = buffers.emplace(blockname, std::move(blockData));

			res.first->second.name = res.first->first;
		}

		char name[nameMaxSize];
		char** names = (char**)alloca(sizeof(char*) * (activeUniforms));
		GLuint* uniformIndices = (GLuint*)alloca(sizeof(GLuint) * activeUniforms);
		GLint* offsets = (GLint*)alloca(sizeof(GLint) * activeUniforms);
		for (uint16_t i = 0; i < activeUniforms; ++i)
		{
			GLsizei nameLength = 0;
			GLint uniformSize = 0;
			GLenum uniformType = 0;

			glGetActiveUniform(shaderId, (GLuint)i, nameMaxSize, &nameLength, &uniformSize, &uniformType, name);

			auto [pair, _] = buffers[baseBufferName].uniforms.emplace(name, Uniform{});
			names[i] = (char*)pair->first.data();

			pair->second.name = pair->first;
			pair->second.type = uniformType;
		}

		if (activeBlocks > 0)
		{
			glGetUniformIndices(shaderId, activeUniforms, names, uniformIndices);
			glGetActiveUniformsiv(shaderId, activeUniforms, uniformIndices, GL_UNIFORM_OFFSET, offsets);
		}

		for (uint16_t i = 0; i < activeUniforms; ++i)
		{
			auto* base = &buffers[baseBufferName];
			if (offsets[i] == -1)
			{
				base->uniforms.at(names[i]).location = glGetUniformLocation(shaderId, names[i]);
				continue;
			}

			bool found = false;
			for (auto& buf : buffers)
			{
				if (uniformIndices[i] >= buf.second.uniformBeginIndex && uniformIndices[i] <= buf.second.uniformEndIndex)
				{
					base = &buf.second;

					found = true;
					break;
				}
			}

			if (found)
			{
				base->uniforms.insert(buffers[baseBufferName].uniforms.extract(names[i]));

				auto& uniform = base->uniforms.at(names[i]);
				uniform.index = uniformIndices[i];
				uniform.offset = offsets[i];
				uniform.buffer = base->bufferMemory;
			}
		}

		for (auto& buf : buffers)
		{
			std::cout << "Buffer:\t" << buf.first << " bi: " << buf.second.binding << ' ' << buf.second.blockSize << " m:"
					  << (int*)buf.second.bufferMemory << "b\n ";

			for (auto& uniform : buf.second.uniforms)
			{
				std::cout << "\t" << uniform.first << ' ' << "i:" << uniform.second.index
						  << " o: " << uniform.second.offset << " l:" << uniform.second.location << '\n';
			}

			std::cout << std::endl;
		}

		std::cout << std::endl;
	}
}

Shader::BlockData::BlockData()
 : blockIndex(-1)
 , uniformBeginIndex(-1)
 , uniformEndIndex(-1)
 , blockSize(-1)
 , binding(-1)
 , bufferBlockId(-1)
 , bufferMemory(nullptr)
{}

Shader::BlockData::BlockData(BlockData&& other) noexcept
 : name(other.name)
 , blockIndex(other.blockIndex)
 , uniformBeginIndex(other.uniformBeginIndex)
 , uniformEndIndex(other.uniformEndIndex)
 , uniforms(std::move(other.uniforms))
 , blockSize(other.blockSize)
 , binding(other.binding)
 , bufferBlockId(other.bufferBlockId)
 , bufferMemory(other.bufferMemory)
{
	other.bufferMemory = nullptr;
}

Shader::BlockData& Shader::BlockData::operator=(BlockData&& other) noexcept
{
	name = other.name;

	blockIndex = other.blockIndex;

	binding = other.binding;

	uniformBeginIndex = other.uniformBeginIndex;
	uniformEndIndex = other.uniformEndIndex;

	blockSize = other.blockSize;

	uniforms = std::move(other.uniforms);
	
	bufferBlockId = other.bufferBlockId;

	bufferMemory = other.bufferMemory;

	other.bufferMemory = nullptr;

	return *this;
}
Shader::BlockData::~BlockData() 
{ delete[] bufferMemory; }
}  // namespace FearEngine::Render::Shaders
