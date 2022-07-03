#include <glad/glad.h>
#include "VertexArray.hpp"

#include <GLFW/glfw3.h>

namespace FearEngine::Render
{
VertexArray::VertexArray()
 : VAO(-1)
 , attribIndex(0)
{}

void VertexArray::genArray() { glGenVertexArrays(1, &VAO); }

void VertexArray::bind() { glBindVertexArray(VAO); }

void VertexArray::unBind() { glBindVertexArray(0); }

void VertexArray::addVertexBuffer(const Buffer& buffer, VertexArrayUpdateType updateType)
{
	int stride = buffer.getStride();
	buffer.bind();
	for (const auto& element : buffer.GetElements())
	{
		if (element.type != BufferType::Int)
		{
			glVertexAttribPointer(attribIndex, element.count, static_cast<GLenum>(element.type), element.normalized, stride,
				 reinterpret_cast<void*>(element.offset));
		}
		else
		{
			glVertexAttribIPointer(
				 attribIndex, element.count, static_cast<GLenum>(element.type), stride, reinterpret_cast<void*>(element.offset));
		}
		glEnableVertexAttribArray(attribIndex);
		glVertexAttribDivisor(attribIndex++, updateType);
	}
	buffer.unbind();
}

VertexArray::~VertexArray() { glDeleteVertexArrays(1, &VAO); }

}  // namespace FearEngine::Render
