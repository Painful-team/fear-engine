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
	struct BlockData
	{
		std::string_view name;

		uint16_t blockIndex;

		uint16_t uniformBeginIndex;
		uint16_t uniformEndIndex;

		std::vector<Uniform> uniforms;

		BlockData() = default;
		BlockData(BlockData&& other) noexcept;
		BlockData& operator=(BlockData&& other) noexcept;
	};

public:
	Shader();

	void readShader(const char* path, GLenum shaderType);
	std::string& getSource(GLenum shaderType);

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
