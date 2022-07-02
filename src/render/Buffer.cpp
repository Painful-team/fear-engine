#include <glad/glad.h>

#include "Buffer.hpp"

namespace FearEngine::Render
{
Buffer::Buffer(std::initializer_list<BufferElement> element)
 : elements(element)
 , VBO(0)
{
	int offset = 0;
	for (auto& it : elements)
	{
		it.offset = offset;
		offset += it.size * it.count;
	}
}

void Buffer::genBuffer() { glGenBuffers(1, &VBO); }

void Buffer::bindData(const uint32_t size)
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

void Buffer::bindData(float* vertices, const uint32_t size)
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

void Buffer::setData(float* vertices, const uint32_t size)
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, vertices);
}

const std::vector<BufferElement>& Buffer::GetElements() const { return elements; }

int Buffer::getStride() const
{
	return elements.back().offset + BufferElement::getSize(elements.back().type) * elements.back().count;
}

void Buffer::unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }
void Buffer::bind() const { glBindBuffer(GL_ARRAY_BUFFER, VBO); }

Buffer::~Buffer() { glDeleteBuffers(1, &VBO); }

BufferElement::BufferElement(const BufferType type, const int count, bool normalized)
 : type(type)
 , normalized(normalized)
 , count(count)
 , size(getSize(type))
{}

int BufferElement::getSize(const BufferType type)
{
	switch (type)
	{
	case BufferType::Float:
		return sizeof(float);
	case BufferType::Short:
		return sizeof(short);
	case BufferType::Int:
		return sizeof(int);
	case BufferType::Char:
		return sizeof(char);
	}

	return 0;
}
}  // namespace FearEngine::Render
