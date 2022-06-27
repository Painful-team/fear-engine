#include "Shader.hpp"
#include <glad/glad.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <core/Engine.hpp>

namespace FearEngine::Render::Shaders
{
Shader::Shader()
 : shaderId(0)
{
	BlockData blockData;
	blockData.blockIndex = -1;

	auto res = buffers.emplace(baseBufferName, std::move(blockData));

	res.first->second.name = res.first->first;
}

errorCode Shader::readShader(const char* path, GLenum shaderType)
{
	Engine::logs()->log("Render", "[Shader System] {0} read has begun.");

	std::string result;
	std::ifstream in(path, std::ios::in);
	if (!in)
	{
		Engine::logs()->error("Render", "[Shader System] Read of {0} has failed with error \"No such file.\"");
		return errorCodes::SHADER_FILE_NOT_EXIST;
	}

	in.seekg(0, std::ios::end);
	size_t size = in.tellg();
	if (size == 0)
	{
		Engine::logs()->error("Render", "[Shader System] Read of {0} has failed with error \"File is empty.\"");
		return errorCodes::SHADER_FILE_EMPTY;
	}

	result.resize(size);
	in.seekg(0, std::ios::beg);
	in.read(result.data(), size);

	sources.emplace(shaderType, std::move(result));

	Engine::logs()->log("Render", "[Shader System] {0} read has ended successfully.");

	return errorCodes::OK;
}

std::string& Shader::getSource(ShaderType shaderType) { return sources[shaderType]; }

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

void Shader::linkBuffer(const std::string& bufferName, Shader& shader)
{
	assert(bufferName == Shader::baseBufferName && "Can't link " && Shader::baseBufferName);
	if (buffers.find(bufferName) == buffers.end())
	{
		buffers.emplace(bufferName, shader.buffers.find(bufferName)->second);
	}
}

std::vector<std::string_view> Shader::getBufferNames() const
{
	std::vector<std::string_view> names;
	names.resize(buffers.size());
	for (auto& buf: buffers)
	{
		names.emplace_back(buf.first);
	}

	return names;
}

void Shader::updateBuffers()
{
	for (auto& buf : buffers)
	{
		if (buf.first == baseBufferName)
		{
			continue;
		}

		// Todo build dynamic memory update function, to decrease number of unnecessary copies
		glBindBuffer(GL_UNIFORM_BUFFER, buf.second.bufferBlockId);

		// for (auto& un : buf.second.uniforms)
		//{
		//	glBufferSubData(GL_UNIFORM_BUFFER, un.second.offset, sizeof(glm::mat4), buf.second.bufferMemory.get() + un.second.offset);
		// }

		glBufferData(GL_UNIFORM_BUFFER, buf.second.blockSize, buf.second.bufferMemory.get(), GL_DYNAMIC_DRAW);

		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
}

errorCode Shader::compile()
{
	uint32_t program = glCreateProgram();

	uint8_t index = 0;
	uint32_t* shaders = reinterpret_cast<uint32_t*>(alloca(sizeof(uint32_t) * sources.size()));
	for (const auto& it : sources)
	{
		uint32_t shader = glCreateShader(it.first);
		const char* shaderCode = it.second.c_str();
		glShaderSource(shader, 1, &shaderCode, NULL);
		glCompileShader(shader);

		int success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
#ifdef _DEBUG
			char infoLog[512];
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			Engine::logs()->error("Render", "[Shader System] Compilation has failed with error \"{0}\"", infoLog);
#endif	// _DEBUG

			for (uint8_t i = 0 ; i < index; ++i)
			{
				glDeleteShader(shaders[i]);
			}

			glDeleteProgram(program);

			return errorCodes::SHADER_COMPILATION_FAILED;
		}

		shaders[index++] = shader;

		glAttachShader(program, shader);
	}

	glLinkProgram(program);

	int success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
#ifdef _DEBUG
		char infoLog[512];
		glGetProgramInfoLog(program, 512, NULL, infoLog);

		for (uint8_t i = 0; i < index; ++i)
		{
			glDeleteShader(shaders[i]);
		}

		glDeleteProgram(program);

		Engine::logs()->error("Render", "[Shader System] Linking has failed with error \"{0}\"", infoLog);
#endif	// _DEBUG

		return errorCodes::SHADER_LINKAGE_FAILED;
	}

	shaderId = program;
	initUniforms();

	for (uint8_t i = 0; i < index; ++i)
	{
		glDeleteShader(shaders[i]);
	}

	return errorCodes::OK;
}

void Shader::use() { glUseProgram(shaderId); }


uint32_t Shader::getId() { return shaderId; }
Shader::~Shader() { glDeleteProgram(shaderId); }

// Add array/struct support
void Shader::initUniforms()
{
	GLint activeUniforms = 0;
	GLint activeBlocks = 0;
	glGetProgramiv(shaderId, GL_ACTIVE_UNIFORMS, &activeUniforms);
	glGetProgramiv(shaderId, GL_ACTIVE_UNIFORM_BLOCKS, &activeBlocks);

	constexpr uint16_t nameMaxSize = 128;

	bool copied = buffers.size() != 1;
	uint32_t copiedCount = 0;
	int startStride = -1;

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
		if (copied)
		{
			bool inCopiedBuffer = false;
			for (auto& buf : buffers)
			{
				if (buf.second.uniforms.find(name) != buf.second.uniforms.end())
				{
					inCopiedBuffer = true;
					break;
				}
			}

			if (inCopiedBuffer)
			{
				++copiedCount;
				if (i == 0 || startStride > -1)
				{
					++startStride;
				}
				else
				{
					names[i] = names[i - 1];
				}

				continue;
			}
		}

		// Remove brackets from uniform name to simplify access
		if (name[nameLength - 1] == ']')
		{
			for (--nameLength; name[nameLength] != '['; --nameLength)
			{}

			name[nameLength] = '\0';
		}

		auto [pair, _] = buffers[baseBufferName].uniforms.emplace(name, Uniform{});
		names[i] = const_cast<char*>(pair->first.data());

		if (startStride != -1)
		{
			for (int32_t j = startStride; j > -1; --j)
			{
				names[j] = names[i];
			}

			startStride = -1;
		}

		pair->second.name = pair->first;
		pair->second.type = uniformType;
		pair->second.arraySize = uniformSize;
	}

	if (activeBlocks > 0 && activeUniforms > copiedCount)
	{
		glGetUniformIndices(shaderId, activeUniforms, names, uniformIndices);
		glGetActiveUniformsiv(shaderId, activeUniforms, uniformIndices, GL_UNIFORM_OFFSET, offsets);
	}

	{
		uint8_t* busyBinding = (uint8_t*)alloca(sizeof(uint8_t) * Engine::getRender()->graphicsData[GL_MAX_UNIFORM_BUFFER_BINDINGS] - 1);
		char blockname[nameMaxSize];
		for (int i = 0; i < activeBlocks; ++i)
		{
			GLsizei blockNameLength = 0;
			glGetActiveUniformBlockName(shaderId, i, nameMaxSize, &blockNameLength, blockname);
			uint32_t blockIndex = glGetUniformBlockIndex(shaderId, blockname);
			GLint binding;
			glGetActiveUniformBlockiv(shaderId, blockIndex, GL_UNIFORM_BLOCK_BINDING, &binding);

			glUniformBlockBinding(shaderId, blockIndex, binding);

			if (buffers.find(blockname) != buffers.end())
			{
				continue;
			}

			GLint uniforms;
			glGetActiveUniformBlockiv(shaderId, blockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS, &uniforms);

			GLint blockSize;
			glGetActiveUniformBlockiv(shaderId, blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);

			GLint* indices = (GLint*)alloca(sizeof(GLint*) * uniforms);
			glGetActiveUniformBlockiv(shaderId, blockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, &*indices);

			BlockData blockData;
			blockData.blockIndex = blockIndex;
			blockData.blockSize = blockSize;
			blockData.binding = binding;

			blockData.bufferMemory = std::shared_ptr<int8_t[]>(new int8_t[blockData.blockSize]);

			glGenBuffers(1, &blockData.bufferBlockId);

			glBindBuffer(GL_UNIFORM_BUFFER, blockData.bufferBlockId);
			glBufferData(GL_UNIFORM_BUFFER, blockData.blockSize, 0, GL_DYNAMIC_DRAW);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);

			glBindBufferRange(GL_UNIFORM_BUFFER, blockData.binding, blockData.bufferBlockId, 0, blockData.blockSize);
			auto res = buffers.emplace(blockname, std::move(blockData));

			//Todo think about reducing looping
			for (uint16_t j = 0; j < uniforms; ++j)
			{
				for (uint16_t k = 0; k < activeUniforms; ++k)
				{
					if (indices[j] == uniformIndices[k])
					{
						//Todo Remove buf.find
						auto inserted = res.first->second.uniforms.insert(buffers[baseBufferName].uniforms.extract(names[k]));

						auto& uniform = inserted.position->second;
						uniform.index = uniformIndices[k];
						uniform.offset = offsets[k];
						uniform.buffer = res.first->second.bufferMemory.get();
						break;
					}
				}
			}

			res.first->second.name = res.first->first;
		}

		//Todo think about reducing that loop too
		for (uint16_t i = 0; i < activeUniforms; ++i)
		{
			auto& it = buffers[baseBufferName].uniforms.find(names[i]);
			if (it != buffers[baseBufferName].uniforms.end())
			{
				it->second.location = glGetUniformLocation(shaderId, names[i]);
			}
		}

#ifdef _DEBUG
		for (auto& buf : buffers)
		{
			Engine::logs()->log("Render", fmt::format(fmt::fg(fmt::color::green), "[Shader System] buffer: {0}, buffer index: {1}, buffer binding: {2}, buffer size: {3}",
				 buf.first, buf.second.blockIndex, buf.second.binding, buf.second.blockSize));

			for (auto& uniform : buf.second.uniforms)
			{
				Engine::logs()->log("Render", fmt::format(fmt::fg(fmt::color::blue), "[Shader System] Uniform: {0}, uniform index: {1}, uniform offset: {2}, uniform location: {3}",
					 uniform.first, uniform.second.index, uniform.second.offset, uniform.second.location));
			}
		}
#endif	// DEBUG
	}
}

Shader::BlockData::BlockData()
 : blockIndex(-1)
 , blockSize(-1)
 , binding(-1)
 , bufferBlockId(-1)
{}

Shader::BlockData::BlockData(BlockData& other)
{ *this = other; }

Shader::BlockData::BlockData(BlockData&& other) noexcept
{
	*this = std::move(other); }

Shader::BlockData& Shader::BlockData::operator=(BlockData& other) noexcept
{
	name = other.name;
	blockIndex = other.blockIndex;
	binding = other.binding;
	blockSize = other.blockSize;
	bufferBlockId = other.bufferBlockId;

	// Todo could be replaced with shared_ptr<string>
	for (auto& un : other.uniforms)
	{
		auto res = uniforms.emplace(un.first, un.second);
		res.first->second.name = res.first->first;
	}

	bufferMemory = other.bufferMemory;

	return *this;
}

Shader::BlockData& Shader::BlockData::operator=(BlockData&& other) noexcept
{
	name = std::move(other.name);

	blockIndex = other.blockIndex;

	binding = other.binding;

	blockSize = other.blockSize;

	uniforms = std::move(other.uniforms);

	bufferBlockId = other.bufferBlockId;

	bufferMemory.swap(other.bufferMemory);
	other.bufferMemory.reset();

	return *this;
}
Shader::BlockData::~BlockData() {}
}  // namespace FearEngine::Render::Shaders
