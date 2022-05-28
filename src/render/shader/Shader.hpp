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
public:
	Shader();

	void readShader(const char* path, GLenum shaderType);
	std::string& getSource(GLenum shaderType);

	void compile();
	void use();

	uint32_t getId();

	~Shader();

private:
	std::unordered_map<GLenum, std::string> sources;
	uint32_t shaderId;

	std::unordered_map<std::string, std::vector<Uniform>> buffers;
};
}  // namespace FearEngine::Render::Shaders
#endif
