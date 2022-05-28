#include <glad/glad.h>
#include "VertexArray.hpp"

#include <GLFW/glfw3.h>

namespace FearEngine::Render
{
VertexArray::VertexArray()
 : VAO(0)
 , attribIndex(0)
{}

void VertexArray::genArray() { glGenVertexArrays(1, &VAO); }

void VertexArray::bind() { glBindVertexArray(VAO); }

void VertexArray::unBind() { glBindVertexArray(0); }

void VertexArray::addVertexBuffer(const VertexBuffer& buffer)
{
	int offset = 0;
	int stride = buffer.getStride();
	for (const auto& element : buffer.GetElements())
	{
		glVertexAttribPointer(attribIndex, element.count, static_cast<GLenum>(element.type), element.normalized, stride,
			 reinterpret_cast<void*>(element.offset));
		glEnableVertexAttribArray(attribIndex++);
	}
}

VertexArray::~VertexArray() { glDeleteVertexArrays(1, &VAO); }

}  // namespace FearEngine::Render
