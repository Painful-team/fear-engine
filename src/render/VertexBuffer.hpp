#ifndef FEARENGINE_RENDER_VERTEXBUFFER_H__
#define FEARENGINE_RENDER_VERTEXBUFFER_H__

#include <vector>

#include <GLFW/glfw3.h>

namespace FearEngine::Render
{
using uint32_t = unsigned;

enum class BufferType
{
	Float = GL_FLOAT,
	Short = GL_SHORT,
	Int = GL_INT,
	Char = GL_BYTE
};

struct BufferElement
{
	int size;
	BufferType type;
	bool normalized;
	uint32_t offset;
	char count;

	BufferElement(const BufferType type, const int count, bool normalized = false);

	static int getSize(const BufferType type);
};

class VertexBuffer
{
public:
	VertexBuffer(std::initializer_list<BufferElement> element);

	void genBuffer();

	void bindData(const uint32_t size);
	void bindData(float* vertices, const uint32_t size);
	void setData(float* vertices, const uint32_t size);

	const std::vector<BufferElement>& GetElements() const;
	int getStride() const;

	void unbind();

	~VertexBuffer();

private:
	uint32_t VBO;

	//Todo could be replaced with just an array
	std::vector<BufferElement> elements;
};
}  // namespace FearEngine::Render

#endif
