#ifndef FEARENGINE_RENDER_SHADER_SHADER_H__
#define FEARENGINE_RENDER_SHADER_SHADER_H__

#include "Uniform.hpp"

#include <string>
#include <vector>
#include <unordered_map>

namespace FearEngine::Render::Shaders
{
using GLenum = unsigned;
using uint32_t = unsigned;

class Shader
{
private:
	using UniformStorage = std::unordered_map<std::string, Uniform>;

	static constexpr const char* baseBufferName = "base";

	struct BlockData
	{
		std::string_view name;

		uint16_t blockIndex;

		uint16_t binding;

		uint32_t uniformBeginIndex;
		uint32_t uniformEndIndex;

		uint16_t blockSize;

		uint32_t bufferBlockId;
		unsigned int uboExampleBlock;

		int8_t* bufferMemory;

		std::unordered_map<std::string, Uniform> uniforms;

		BlockData();
		BlockData(BlockData&& other) noexcept;
		BlockData& operator=(BlockData&& other) noexcept;

		~BlockData();
	};

public:
	Shader();

	void readShader(const char* path, GLenum shaderType);
	std::string& getSource(GLenum shaderType);

	Uniform& findUniform(const std::string& name);
	UniformStorage& findBuffer(const std::string& name);

	void updateBuffers();
	void compile();
	void use();

	uint32_t getId();

	~Shader();

private:
	void initUniforms();

	std::unordered_map<GLenum, std::string> sources;
	uint32_t shaderId;

	std::unordered_map<std::string, BlockData> buffers;
};
}  // namespace FearEngine::Render::Shaders
#endif
