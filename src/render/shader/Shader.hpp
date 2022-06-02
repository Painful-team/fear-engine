#ifndef FEARENGINE_RENDER_SHADER_SHADER_H__
#define FEARENGINE_RENDER_SHADER_SHADER_H__

#include "Uniform.hpp"

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

namespace FearEngine::Render::Shaders
{
using GLenum = uint32_t;

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

		std::shared_ptr<int8_t[]> bufferMemory;

		std::unordered_map<std::string, Uniform> uniforms;

		BlockData();
		BlockData(BlockData& other);
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

	void linkBuffer(const std::string& bufferName, Shader& shader);

	std::vector<std::string_view> getBufferNames() const;

	void updateBuffers();
	void compile();
	void use();

	uint32_t getId();

	~Shader();

	void initUniforms();

	std::unordered_map<GLenum, std::string> sources;
	uint32_t shaderId;

	std::unordered_map<std::string, BlockData> buffers;
private:
};
}  // namespace FearEngine::Render::Shaders
#endif
