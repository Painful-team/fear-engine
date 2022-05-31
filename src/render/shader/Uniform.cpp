#include "Uniform.hpp"

#include <glad/glad.h>

#include <iostream>
#include <render/ErrorCodes.hpp>

namespace FearEngine::Render::Shaders
{
void Uniform::setBool(const bool value) const
{
	assert(!name.empty() && "Uniform not initialized");

	glUniform1i(location, static_cast<int>(value));
}

void Uniform::setInt(const int value) const
{
	assert(!name.empty() && "Uniform not initialized");

	glUniform1i(location, value);
}

void Uniform::setFloat(const float value) const
{
	assert(!name.empty() && "Uniform not initialized");
	if (location != -1)
	{
		glUniform1f(location, value);
	}
	else
	{
		*reinterpret_cast<float*>(buffer + offset) = value;
	}
}

void Uniform::setVec2(const glm::vec2& value) const
{
	assert(!name.empty() && "Uniform not initialized");

	glUniform2fv(location, 1, &value[0]);
}

void Uniform::setVec2(const float x, const float y) const
{
	assert(!name.empty() && "Uniform not initialized");

	glUniform2f(location, x, y);
}

void Uniform::setVec3(const glm::vec3& value) const
{
	assert(!name.empty() && "Uniform not initialized");

	glUniform3fv(location, 1, &value[0]);
}

void Uniform::setVec3(const float x, const float y, const float z) const
{
	assert(!name.empty() && "Uniform not initialized");

	glUniform3f(location, x, y, z);
}

void Uniform::setVec4(const glm::vec4& value) const
{
	assert(!name.empty() && "Uniform not initialized");

	glUniform4fv(location, 1, &value[0]);
}

void Uniform::setVec4(const float x, const float y, const float z, const float w) const
{
	assert(!name.empty() && "Uniform not initialized");

	glUniform4f(location, x, y, z, w);
}

void Uniform::setMat2(const glm::mat2& mat) const
{
	assert(!name.empty() && "Uniform not initialized");

	glUniformMatrix2fv(location, 1, false, &mat[0][0]);
}

void Uniform::setMat3(const glm::mat3& mat) const
{
	assert(!name.empty() && "Uniform not initialized");

	glUniformMatrix3fv(location, 1, false, &mat[0][0]);
}

void Uniform::setMat4(const glm::mat4& mat) const
{
	assert(!name.empty() && "Uniform not initialized");

	if (location != -1)
	{
		glUniformMatrix4fv(location, 1, false, &mat[0][0]);
	}
	else
	{
		*reinterpret_cast<glm::mat4*>(buffer + offset) = mat;
	}
}

Uniform::Uniform()
 : location(-1)
 , index(-1)
 , offset(-1)
 , type(-1)
 , buffer(nullptr)
{}

Uniform::Uniform(const Uniform& other) { *this = other; }

Uniform::Uniform(Uniform&& other) noexcept
 : name(std::move(other.name))
 , location(other.location)
 , index(other.index)
 , offset(other.offset)
 , type(other.type)
 , buffer(other.buffer)
{
	other.buffer = nullptr;
}

Uniform& Uniform::operator=(const Uniform& other)
{
	name = other.name;
	location = other.location;
	index = other.index;
	offset = other.offset;
	type = other.type;
	buffer = other.buffer;

	return *this;
}

Uniform& Uniform::operator=(Uniform&& other) noexcept
{
	name = std::move(other.name);
	location = other.location;
	index = other.index;
	offset = other.offset;
	type = other.type;
	buffer = other.buffer;
	other.buffer = nullptr;

	return *this;
}

const std::string_view& Uniform::getName() const { return name; }

int Uniform::isValid() { return !name.empty(); }
}  // namespace FearEngine::Render::Shaders
