#include <glad/glad.h>

#include "VertexBuffer.hpp"

namespace FearEngine::Render
{
VertexBuffer::VertexBuffer(std::initializer_list<VertexBufferElement> element)
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

void VertexBuffer::bindData(void* data, const uint32_t size)
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void VertexBuffer::setData(void* data, const uint32_t size)
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}

const std::vector<VertexBufferElement>& VertexBuffer::GetElements() const { return elements; }

int VertexBuffer::getStride() const
{
	return elements.back().offset + VertexBufferElement::getSize(elements.back().type) * elements.back().count;
}

void VertexBuffer::unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }
void VertexBuffer::bind() const { glBindBuffer(GL_ARRAY_BUFFER, VBO); }

VertexBuffer::~VertexBuffer() { glDeleteBuffers(1, &VBO); }

VertexBufferElement::VertexBufferElement(const VertexBufferType type, const int count, bool normalized)
 : type(type)
 , normalized(normalized)
 , count(count)
 , size(getSize(type))
{}

int VertexBufferElement::getSize(const VertexBufferType type)
{
	switch (type)
	{
	case VertexBufferType::Float:
		return sizeof(float);
	case VertexBufferType::Short:
		return sizeof(short);
	case VertexBufferType::Int:
		return sizeof(int);
	case VertexBufferType::Char:
		return sizeof(char);
	}

	return 0;
}
}  // namespace FearEngine::Render
