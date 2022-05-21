#include "Uniform.hpp"

#include <glad/glad.h>

#include <render/ErrorCodes.hpp>

namespace FearEngine::Render::Shaders
{
	void Uniform::setBool(const bool value) const
	{
		assert(m_location != -1 && "Uniform not initialized");

		glUniform1i(m_location, static_cast<int>(value));
	}

	void Uniform::setInt(const int value) const
	{
		assert(m_location != -1 && "Uniform not initialized");

		glUniform1i(m_location, value);
	}

	void Uniform::setFloat(const float value) const
	{
		assert(m_location != -1 && "Uniform not initialized");

		glUniform1f(m_location, value);
	}

	void Uniform::setVec2(const glm::vec2& value) const
	{
		assert(m_location != -1 && "Uniform not initialized");

		glUniform2fv(m_location, 1, &value[0]);
	}

	void Uniform::setVec2(const float x, const float y) const
	{
		assert(m_location != -1 && "Uniform not initialized");

		glUniform2f(m_location, x, y);
	}

	void Uniform::setVec3(const glm::vec3& value) const
	{
		assert(m_location != -1 && "Uniform not initialized");

		glUniform3fv(m_location, 1, &value[0]);
	}

	void Uniform::setVec3(const float x, const float y, const float z) const
	{
		assert(m_location != -1 && "Uniform not initialized");

		glUniform3f(m_location, x, y, z);
	}

	void Uniform::setVec4(const glm::vec4& value) const
	{
		assert(m_location != -1 && "Uniform not initialized");

		glUniform4fv(m_location, 1, &value[0]);
	}

	void Uniform::setVec4(const float x, const float y, const float z, const float w) const
	{
		assert(m_location != -1 && "Uniform not initialized");

		glUniform4f(m_location, x, y, z, w);
	}

	void Uniform::setMat2(const glm::mat2& mat) const
	{
		assert(m_location != -1 && "Uniform not initialized");

		glUniformMatrix2fv(m_location, 1, false, &mat[0][0]);
	}

	void Uniform::setMat3(const glm::mat3& mat) const
	{
		assert(m_location != -1 && "Uniform not initialized");

		glUniformMatrix3fv(m_location, 1, false, &mat[0][0]);
	}

	void Uniform::setMat4(const glm::mat4& mat) const
	{
		assert(m_location != -1 && "Uniform not initialized");

		glUniformMatrix4fv(m_location, 1, false, &mat[0][0]);
	}

	Uniform::Uniform() :
		m_location(-1)
	{}

	Uniform::Uniform(const uint32_t program, const char* name)
	{
		m_location = glGetUniformLocation(program, name);
	}

	Uniform::Uniform(const Uniform& other) :
		m_location(other.m_location)
	{}

	Uniform& Uniform::operator=(const Uniform& other)
	{
		m_location = other.m_location;

		return *this;
	}

	int Uniform::init(const uint32_t program, const char* name)
	{
		m_location = glGetUniformLocation(program, name);

		return m_location == -1 ? 0: FearEngine::Render::errorCodes::Uniform_Location_Not_Found;
	}
}
