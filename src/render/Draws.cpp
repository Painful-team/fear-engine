#include "glad/glad.h"
#include "Draws.hpp"

#include <core/Engine.hpp>

void FearEngine::Render::Draws::draw(VertexArray& arr, uint32_t verticesCount)
{
	arr.bind();
	glDrawArrays(GL_TRIANGLES, 0, verticesCount);

	++Engine::getRender()->stats.drawCalls;
	Engine::getRender()->stats.polygons += verticesCount / 3;
}

void FearEngine::Render::Draws::drawIndexed(VertexArray& arr, uint32_t verticesCount, uint32_t objCount)
{
	arr.bind();
	glDrawArraysInstanced(GL_TRIANGLES, 0, verticesCount, objCount);

	++Engine::getRender()->stats.drawCalls;
	Engine::getRender()->stats.polygons += verticesCount * objCount / 3;
}
