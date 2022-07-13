#ifndef FEARENGINE_RENDER_VERTEXBUFFER_H__
#define FEARENGINE_RENDER_VERTEXBUFFER_H__

#include <vector>

#include <GLFW/glfw3.h>

namespace FearEngine::Render
{
using uint32_t = unsigned;

enum class VertexBufferType
{
	Float = GL_FLOAT,
	Short = GL_SHORT,
	Int = GL_INT,
	Char = GL_BYTE,
	Mat4,
};

struct VertexBufferElement
{
	int size;
	VertexBufferType type;
	bool normalized;
	uint32_t offset;
	char count;

	VertexBufferElement(const VertexBufferType type, const int count, bool normalized = false);

	static int getSize(const VertexBufferType type);
};

class VertexBuffer
{
public:
	VertexBuffer(std::initializer_list<VertexBufferElement> element);

	void genBuffer();
	void bind() const;

	void bindData(const uint32_t size);
	void bindData(void* data, const uint32_t size);
	void setData(void* data, const uint32_t size);

	const std::vector<VertexBufferElement>& GetElements() const;
	int getStride() const;

	void unbind() const;

	~VertexBuffer();

private:
	uint32_t VBO;

	// Todo could be replaced with just an array
	std::vector<VertexBufferElement> elements;
};
}  // namespace FearEngine::Render

#endif
