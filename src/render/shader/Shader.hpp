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

namespace FearEngine
{
class Renderer;
}

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
	struct ShaderBufferData
	{
		std::string_view name;

		uint16_t blockIndex;

		uint16_t binding;

		uint16_t blockSize;

		uint32_t bufferBlockId;

		int8_t* bufferMemory;

		std::unordered_map<std::string, Uniform> uniforms;

		ShaderBufferData();
		ShaderBufferData(ShaderBufferData& other);
		ShaderBufferData(ShaderBufferData&& other) noexcept;
		ShaderBufferData& operator=(ShaderBufferData& other) noexcept;
		ShaderBufferData& operator=(ShaderBufferData&& other) noexcept;

		~ShaderBufferData();
	};

public:
	static constexpr uint32_t maxTextureSlots = 32;
	using UniformStorage = std::unordered_map<std::string, Uniform>;

	Shader();

	errorCode readShader(const std::string& path, GLenum shaderType);
	std::string& getSource(ShaderType shaderType);

	Uniform& findUniform(const std::string& name);
	UniformStorage& findBuffer(const std::string& name);

	std::vector<std::string_view> getBufferNames() const;

	void updateBuffers();
	errorCode compile();
	void use();

	uint32_t getId();

	~Shader();


private:
	void initUniforms();

	std::unordered_map<GLenum, std::string> sources;
	uint32_t shaderId;

	ShaderBufferData defaultBuffer;

	std::unordered_map<std::string, std::shared_ptr<ShaderBufferData>> buffers;

	friend class Renderer;
};
}  // namespace FearEngine::Render::Shaders
#endif
