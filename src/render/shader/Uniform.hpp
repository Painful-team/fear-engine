#ifndef FEARENGINE_RENDER_SHADER_UNIFORM_H__
#define FEARENGINE_RENDER_SHADER_UNIFORM_H__

#include <glm/glm.hpp>
#include <string>

#include "UniformTypes.hpp"

namespace FearEngine::Render::Shaders
{

class Uniform
{
public:
	Uniform();

	Uniform(const Uniform& other);
	Uniform(Uniform&& other) noexcept;

	Uniform& operator=(const Uniform& other);
	Uniform& operator=(Uniform&& other) noexcept;

	int isValid();

	uniformType getType() const;
	uint16_t getTypeSize() const;
	const std::string_view& getName() const;

	bool isArray() const;
	int getArraySize() const;

	void setInt(const int value) const;

	void setVec2(const float x, const float y) const;
	void setVec3(const float x, const float y, const float z) const;
	void setVec4(const float x, const float y, const float z, const float w) const;

	void setBool(const bool* const value, uint32_t count = 1, uint32_t offset = 0) const;
	void setInt(const int* const value, uint32_t count = 1, uint32_t offset = 0) const;
	void setFloat(const float* const value, uint32_t count = 1, uint32_t offset = 0) const;

	void setVec2(const glm::vec2* const value, uint32_t count = 1, uint32_t offset = 0) const;
	void setMat2(const glm::mat2* const mat, uint32_t count = 1, uint32_t offset = 0) const;

	void setVec3(const glm::vec3* const value, uint32_t count = 1, uint32_t offset = 0) const;
	void setMat3(const glm::mat3* const mat, uint32_t count = 1, uint32_t offset = 0) const;

	void setVec4(const glm::vec4* const value, uint32_t count = 1, uint32_t offset = 0) const;
	void setMat4(const glm::mat4* const mat, uint32_t count = 1, uint32_t offset = 0) const;

	// Pack data into int array for uniforms except uniform buffers
	void setData(void* data, uint32_t size, uint32_t offset) const;

private:
	std::string_view name;

	uniformType type;

	// Todo think about replacement with union
	uint32_t location;

	uint32_t arraySize;

	uint32_t index;
	uint32_t offset;

	int8_t* buffer;

	friend class Shader;
};
}  // namespace FearEngine::Render::Shaders
#endif
