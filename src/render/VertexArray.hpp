#ifndef FEARENGINE_RENDER_VERTEXARRAY_H__
#define FEARENGINE_RENDER_VERTEXARRAY_H__

#include "VertexBuffer.hpp"

namespace FearEngine::Render
{
using uint32_t = unsigned;
class VertexArray
{
public:
	VertexArray();

	void genArray();

	void bind();
	void unBind();

	void addVertexBuffer(const VertexBuffer& buffer);

	~VertexArray();

private:
	uint32_t VAO;
	uint32_t attribIndex;
};
}  // namespace FearEngine::Render

#endif
