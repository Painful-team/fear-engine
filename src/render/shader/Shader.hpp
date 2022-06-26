#ifndef FEARENGINE_RENDER_SHADER_SHADER_H__
#define FEARENGINE_RENDER_SHADER_SHADER_H__

#include <glad/glad.h>

#include "Uniform.hpp"

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

#include <render/ErrorCodes.hpp>

#include <render/Texture.hpp>

namespace FearEngine::Render::Shaders
{
enum ShaderType
{
	None = 0,
	Vertex = GL_VERTEX_SHADER,
	Fragment = GL_FRAGMENT_SHADER,
	Geometry = GL_GEOMETRY_SHADER
};

class Shader
{
private:
	//Todo think about replacing it on dynamic value even in the shaders
	static constexpr const char* baseBufferName = "base";

	struct BlockData
	{
		std::string_view name;

		uint16_t blockIndex;

		uint16_t binding;

		uint16_t blockSize;

		uint32_t bufferBlockId;

		std::shared_ptr<int8_t[]> bufferMemory;

		std::unordered_map<std::string, Uniform> uniforms;

		BlockData();
		BlockData(BlockData& other);
		BlockData(BlockData&& other) noexcept;
		BlockData& operator=(BlockData& other) noexcept;
		BlockData& operator=(BlockData&& other) noexcept;

		~BlockData();
	};

public:
	static constexpr uint32_t maxTextureSlots = 32;
	using UniformStorage = std::unordered_map<std::string, Uniform>;

	Shader();

	errorCode readShader(const char* path, GLenum shaderType);
	std::string& getSource(ShaderType shaderType);

	Uniform& findUniform(const std::string& name);
	UniformStorage& findBuffer(const std::string& name);

	void linkBuffer(const std::string& bufferName, Shader& shader);

	std::vector<std::string_view> getBufferNames() const;

	void updateBuffers();
	errorCode compile();
	void use();

	uint32_t getId();

	~Shader();

	void initUniforms();

private:
	std::unordered_map<GLenum, std::string> sources;
	uint32_t shaderId;

	std::unordered_map<std::string, BlockData> buffers;
};
}  // namespace FearEngine::Render::Shaders
#endif
