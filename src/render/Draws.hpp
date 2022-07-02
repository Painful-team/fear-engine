#ifndef FEARENGINE_RENDER_DRAWS_H__
#define FEARENGINE_RENDER_DRAWS_H__

#include "VertexArray.hpp"

namespace FearEngine::Render::Draws
{
void draw(VertexArray& arr, uint32_t verticesCount);
void drawIndexed(VertexArray& arr, uint32_t verticesCount, uint32_t objCount);
}

#endif
