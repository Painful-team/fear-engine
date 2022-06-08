#include "Uniform.hpp"

#include <glad/glad.h>

#include <render/ErrorCodes.hpp>

namespace FearEngine::Render::Shaders
{
int Uniform::getArraySize() const { return arraySize; }
void Uniform::setBool(const bool value) const
{
	assert(!name.empty() && "Uniform not initialized");
	if (location != -1)
	{
		glUniform1i(location, static_cast<int>(value));
	}
	else
	{
		*reinterpret_cast<int*>(buffer + offset) = value;
	}
}

void Uniform::setInt(const int value) const
{
	assert(!name.empty() && "Uniform not initialized");
	if (location != -1)
	{
		glUniform1i(location, value);
	}
	else
	{
		*reinterpret_cast<int*>(buffer + offset) = value;
	}
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
	if (location != -1)
	{
		glUniform2fv(location, 1, &value[0]);
	}
	else
	{
		*reinterpret_cast<glm::vec2*>(buffer + offset) = value;
	}
}

void Uniform::setVec2(const float x, const float y) const
{
	assert(!name.empty() && "Uniform not initialized");
	if (location != -1)
	{
		glUniform2f(location, x, y);
	}
	else
	{
		*reinterpret_cast<glm::vec2*>(buffer + offset) = {x, y};
	}
}

void Uniform::setVec3(const glm::vec3& value) const
{
	assert(!name.empty() && "Uniform not initialized");
	if (location != -1)
	{
		glUniform3fv(location, 1, &value[0]);
	}
	else
	{
		*reinterpret_cast<glm::vec3*>(buffer + offset) = value;
	}
}

void Uniform::setVec3(const float x, const float y, const float z) const
{
	assert(!name.empty() && "Uniform not initialized");
	if (location != -1)
	{
		glUniform3f(location, x, y, z);
	}
	else
	{
		*reinterpret_cast<glm::vec3*>(buffer + offset) = {x, y, z};
	}
}

void Uniform::setVec4(const glm::vec4& value) const
{
	assert(!name.empty() && "Uniform not initialized");
	if (location != -1)
	{
		glUniform4fv(location, 1, &value[0]);
	}
	else
	{
		*reinterpret_cast<glm::vec4*>(buffer + offset) = value;
	}
}

void Uniform::setVec4(const float x, const float y, const float z, const float w) const
{
	assert(!name.empty() && "Uniform not initialized");
	if (location != -1)
	{
		glUniform4f(location, x, y, z, w);
	}
	else
	{
		*reinterpret_cast<glm::vec4*>(buffer + offset) = {x, y, z, w};
	}
}

void Uniform::setMat2(const glm::mat2& mat) const
{
	assert(!name.empty() && "Uniform not initialized");
	if (location != -1)
	{
		glUniformMatrix2fv(location, 1, false, &mat[0][0]);
	}
	else
	{
		*reinterpret_cast<glm::mat2*>(buffer + offset) = mat;
	}
}

void Uniform::setMat3(const glm::mat3& mat) const
{
	assert(!name.empty() && "Uniform not initialized");
	if (location != -1)
	{
		glUniformMatrix3fv(location, 1, false, &mat[0][0]);
	}
	else
	{
		*reinterpret_cast<glm::mat3*>(buffer + offset) = mat;
	}
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
 , arraySize(-1)
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
 , arraySize(other.arraySize)
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
	arraySize = other.arraySize;

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
	arraySize = other.arraySize;

	other.buffer = nullptr;

	return *this;
}

const std::string_view& Uniform::getName() const { return name; }

bool Uniform::isArray() const { return arraySize > 1; }

uniformType Uniform::getType() const { return type; }

uint16_t Uniform::getTypeSize() const 
{ 
	switch (type)
	{
	case uniformTypes::FLOAT:
		return sizeof(float);
	case uniformTypes::INT:
		return sizeof(int);
	case uniformTypes::VEC2:
		return sizeof(glm::vec2);
	case uniformTypes::VEC3:
		return sizeof(glm::vec3);
	case uniformTypes::VEC4:
		return sizeof(glm::vec4);
	case uniformTypes::MAT2:
		return sizeof(glm::mat2);
	case uniformTypes::MAT3:
		return sizeof(glm::mat3);
	case uniformTypes::MAT4:
		return sizeof(glm::mat4);
	default: return 0;
	}
}

int Uniform::isValid() { return !name.empty(); }

void Uniform::setBool(bool* const value, uint32_t count) const
{
	assert(!name.empty() && "Uniform not initialized");
	assert(arraySize > count && "Array size is smaller than count");

	if (location != -1)
	{
		glUniform1iv(location, count, reinterpret_cast<int*>(value));
	}
	else
	{
		memcpy(buffer + offset, value, count * sizeof(int));
	}
}

void Uniform::setInt(int* const value, uint32_t count) const
{
	assert(!name.empty() && "Uniform not initialized");
	assert(arraySize >= count && "Array size is smaller than count");

	if (location != -1)
	{
		glUniform1iv(location, count, value);
	}
	else
	{
		memcpy(buffer + offset, value, count * sizeof(int));
	}
}

void Uniform::setFloat(float* const value, uint32_t count) const
{
	assert(!name.empty() && "Uniform not initialized");
	assert(arraySize > count && "Array size is smaller than count");

	if (location != -1)
	{
		glUniform1fv(location, count, value);
	}
	else
	{
		memcpy(buffer + offset, value, count * sizeof(float));
	}
}

void Uniform::setVec2(glm::vec2* const value, uint32_t count) const
{
	assert(!name.empty() && "Uniform not initialized");
	assert(arraySize > count && "Array size is smaller than count");

	if (location != -1)
	{
		glUniform2fv(location, count, &(*value)[0]);
	}
	else
	{
		memcpy(buffer + offset, value, count * sizeof(glm::vec2));
	}
}

void Uniform::setMat2(glm::mat2* const mat, uint32_t count) const
{
	assert(!name.empty() && "Uniform not initialized");
	assert(arraySize > count && "Array size is smaller than count");

	if (location != -1)
	{
		glUniformMatrix2fv(location, count, false, &(*mat)[0][0]);
	}
	else
	{
		memcpy(buffer + offset, mat, count * sizeof(glm::mat2));
	}
}

void Uniform::setVec3(glm::vec3* const value, uint32_t count) const
{
	assert(!name.empty() && "Uniform not initialized");
	assert(arraySize > count && "Array size is smaller than count");

	if (location != -1)
	{
		glUniform3fv(location, count, &(*value)[0]);
	}
	else
	{
		memcpy(buffer + offset, value, count * sizeof(glm::vec3));
	}
}

void Uniform::setMat3(glm::mat3* const mat, uint32_t count) const
{
	assert(!name.empty() && "Uniform not initialized");
	assert(arraySize > count && "Array size is smaller than count");

	if (location != -1)
	{
		glUniformMatrix3fv(location, count, false, &(*mat)[0][0]);
	}
	else
	{
		memcpy(buffer + offset, mat, count * sizeof(glm::mat3));
	}
}

void Uniform::setVec4(glm::vec4* const value, uint32_t count) const
{
	assert(!name.empty() && "Uniform not initialized");
	assert(arraySize > count && "Array size is smaller than count");

	if (location != -1)
	{
		glUniform4fv(location, count, &(*value)[0]);
	}
	else
	{
		memcpy(buffer + offset, value, count * sizeof(glm::vec4));
	}
}

void Uniform::setMat4(glm::mat4* const mat, uint32_t count) const
{
	assert(!name.empty() && "Uniform not initialized");
	assert(arraySize > count && "Array size is smaller than count");

	if (location != -1)
	{
		glUniformMatrix4fv(location, count, false, &(*mat)[0][0]);
	}
	else
	{
		memcpy(buffer + offset, mat, count * sizeof(glm::mat4));
	}
}
}  // namespace FearEngine::Render::Shaders
