#include "IndexBuffer.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace FearEngine::Render
{
	IndexBuffer::IndexBuffer()
	{
		//glGenBuffers(1, &EBO);
	}

	void IndexBuffer::genBuffer()
	{
		glGenBuffers(1, &EBO);
	}

	void IndexBuffer::bindData(const uint32_t size)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	void IndexBuffer::bindData(uint32_t* indices, const uint32_t size)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
	}

	void IndexBuffer::setData(uint32_t* indices, const uint32_t size)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, size, indices);
	}

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &EBO);
	}
}
