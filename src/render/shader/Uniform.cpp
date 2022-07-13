#include "Uniform.hpp"

#include <glad/glad.h>

#include <render/ErrorCodes.hpp>

namespace FearEngine::Render::Shaders
{
int Uniform::getArraySize() const { return arraySize; }

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

Uniform::Uniform()
 : location(-1)
 , index(-1)
 , offset(-1)
 , type(-1)
 , arraySize(-1)
 , buffer(nullptr)
{}

Uniform::Uniform(const Uniform& other) { *this = other; }

Uniform::Uniform(Uniform&& other) noexcept { *this = std::move(other); }

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
	default:
		return 0;
	}
}

int Uniform::isValid() { return !name.empty(); }

void Uniform::setBool(const bool* const value, uint32_t count, uint32_t dataOffset) const
{
	assert(!name.empty() && "Uniform not initialized");
	assert(arraySize >= count && "Array size is smaller than count");

	if (location != -1)
	{
		glUniform1iv(location, count, reinterpret_cast<const int*>(value));
	}
	else
	{
		memcpy(buffer + offset + dataOffset, value, count * sizeof(int));
	}
}

void Uniform::setInt(const int* const value, uint32_t count, uint32_t dataOffset) const
{
	assert(!name.empty() && "Uniform not initialized");
	assert(arraySize >= count && "Array size is smaller than count");

	if (location != -1)
	{
		glUniform1iv(location, count, value);
	}
	else
	{
		memcpy(buffer + offset + dataOffset, value, count * sizeof(int));
	}
}

void Uniform::setFloat(const float* const value, uint32_t count, uint32_t dataOffset) const
{
	assert(!name.empty() && "Uniform not initialized");
	assert(arraySize >= count && "Array size is smaller than count");

	if (location != -1)
	{
		glUniform1fv(location, count, value);
	}
	else
	{
		memcpy(buffer + offset + dataOffset, value, count * sizeof(float));
	}
}

void Uniform::setVec2(const glm::vec2* const value, uint32_t count, uint32_t dataOffset) const
{
	assert(!name.empty() && "Uniform not initialized");
	assert(arraySize >= count && "Array size is smaller than count");

	if (location != -1)
	{
		glUniform2fv(location, count, &(*value)[0]);
	}
	else
	{
		memcpy(buffer + offset + dataOffset, value, count * sizeof(glm::vec2));
	}
}

void Uniform::setMat2(const glm::mat2* const mat, uint32_t count, uint32_t dataOffset) const
{
	assert(!name.empty() && "Uniform not initialized");
	assert(arraySize >= count && "Array size is smaller than count");

	if (location != -1)
	{
		glUniformMatrix2fv(location, count, false, &(*mat)[0][0]);
	}
	else
	{
		memcpy(buffer + offset + dataOffset, mat, count * sizeof(glm::mat2));
	}
}

void Uniform::setVec3(const glm::vec3* const value, uint32_t count, uint32_t dataOffset) const
{
	assert(!name.empty() && "Uniform not initialized");
	assert(arraySize >= count && "Array size is smaller than count");

	if (location != -1)
	{
		glUniform3fv(location, count, &(*value)[0]);
	}
	else
	{
		memcpy(buffer + offset + dataOffset, value, count * sizeof(glm::vec3));
	}
}

void Uniform::setMat3(const glm::mat3* const mat, uint32_t count, uint32_t dataOffset) const
{
	assert(!name.empty() && "Uniform not initialized");
	assert(arraySize >= count && "Array size is smaller than count");

	if (location != -1)
	{
		glUniformMatrix3fv(location, count, false, &(*mat)[0][0]);
	}
	else
	{
		memcpy(buffer + offset + dataOffset, mat, count * sizeof(glm::mat3));
	}
}

void Uniform::setVec4(const glm::vec4* const value, uint32_t count, uint32_t dataOffset) const
{
	assert(!name.empty() && "Uniform not initialized");
	assert(arraySize >= count && "Array size is smaller than count");

	if (location != -1)
	{
		glUniform4fv(location, count, &(*value)[0]);
	}
	else
	{
		memcpy(buffer + offset + dataOffset, value, count * sizeof(glm::vec4));
	}
}

void Uniform::setMat4(const glm::mat4 const* mat, uint32_t count, uint32_t dataOffset) const
{
	assert(!name.empty() && "Uniform not initialized");
	assert(arraySize >= count && "Array size is smaller than count");

	if (location != -1)
	{
		glUniformMatrix4fv(location, count, false, &(*mat)[0][0]);
	}
	else
	{
		memcpy(buffer + offset + dataOffset, mat, count * sizeof(glm::mat4));
	}
}

void Uniform::setData(void* data, uint32_t size, uint32_t dataOffset) const
{
	assert(!name.empty() && "Uniform not initialized");
	assert(getTypeSize() * arraySize >= size && "Array size is smaller than count");

	if (location != -1)
	{
		glUniform1iv(location, size / sizeof(int), reinterpret_cast<int*>(data));
	}
	else
	{
		memcpy(buffer + offset + dataOffset, data, size);
	}
}
}  // namespace FearEngine::Render::Shaders
