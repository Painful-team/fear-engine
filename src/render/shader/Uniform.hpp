#ifndef FEARENGINE_RENDER_SHADER_UNIFORM_H__
#define FEARENGINE_RENDER_SHADER_UNIFORM_H__

#include <glm/glm.hpp>

namespace FearEngine::Render::Shaders
{
class Uniform
{
public:
	Uniform();

	Uniform(const uint32_t program, const char* name);
	Uniform(const Uniform& other);

	Uniform& operator=(const Uniform& other);

	int init(const uint32_t program, const char* name);

	void setBool(const bool value) const;
	void setInt(const int value) const;
	void setFloat(const float value) const;

	void setVec2(const glm::vec2& value) const;
	void setVec2(const float x, const float y) const;
	void setMat2(const glm::mat2& mat) const;

	void setVec3(const glm::vec3& value) const;
	void setVec3(const float x, const float y, const float z) const;
	void setMat3(const glm::mat3& mat) const;

	void setVec4(const glm::vec4& value) const;
	void setVec4(const float x, const float y, const float z, const float w) const;
	void setMat4(const glm::mat4& mat) const;

private:
	uint32_t m_location;
};
}  // namespace FearEngine::Render::Shaders
#endif
