#ifndef FEARENGINE_RENDER_DRAWS_H__
#define FEARENGINE_RENDER_DRAWS_H__

#include "VertexArray.hpp"

#include <components/CameraComponent.hpp>

namespace FearEngine::Render::Draws
{
void draw(VertexArray& arr, uint32_t verticesCount);
void drawIndexed(VertexArray& arr, uint32_t verticesCount, uint32_t objCount);

#pragma pack(push, 1)
struct OutlineVertice
{
	glm::vec3 pos;
	glm::vec3 color;
};
#pragma pack(pop)

errorCode initOutlines();
void clearOutlines();
void drawOutlines(OutlineVertice* vertices, uint32_t size);
void submitOutlines(Component::Camera& cam);
}  // namespace FearEngine::Render::Draws

#endif
