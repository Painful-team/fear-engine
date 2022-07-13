#ifndef FEARENGINE_RENDER_VERTEXARRAY_H__
#define FEARENGINE_RENDER_VERTEXARRAY_H__

#include "VertexBuffer.hpp"

namespace FearEngine::Render
{
using VertexArrayUpdateType = short;
namespace VertexArrayUpdateTypes
{
enum : short
{
	None = 0,
	OnDraw = 1
};
};

class VertexArray
{
public:
	VertexArray();

	void genArray();

	void bind();
	void unBind();

	void addVertexBuffer(const VertexBuffer& buffer, VertexArrayUpdateType updateType = VertexArrayUpdateTypes::None);

	~VertexArray();

private:
	uint32_t VAO;
	uint32_t attribIndex;
};
}  // namespace FearEngine::Render

#endif
