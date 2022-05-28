#include <glad/glad.h>

#include "VertexBuffer.hpp"

namespace FearEngine::Render
{
VertexBuffer::VertexBuffer(std::initializer_list<BufferElement> element)
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

void VertexBuffer::genBuffer() { glGenBuffers(1, &VBO); }

void VertexBuffer::bindData(const uint32_t size)
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

void VertexBuffer::bindData(float* vertices, const uint32_t size)
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

void VertexBuffer::setData(float* vertices, const uint32_t size)
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glGetBufferSubData(GL_ARRAY_BUFFER, 0, size, vertices);
}

const std::vector<BufferElement>& VertexBuffer::GetElements() const { return elements; }

int VertexBuffer::getStride() const
{
	return elements.back().offset + BufferElement::getSize(elements.back().type) * elements.back().count;
}

void VertexBuffer::unbind() { glBindBuffer(GL_ARRAY_BUFFER, 0); }

VertexBuffer::~VertexBuffer() { glDeleteBuffers(1, &VBO); }

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
