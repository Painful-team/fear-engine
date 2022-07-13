#ifndef FEARENGINE_RENDER_EDITORMODELLAYER_H__
#define FEARENGINE_RENDER_EDITORMODELLAYER_H__

#include <memory>

#include "Layer.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "shader/Shader.hpp"

#include <cache/MaterialResource.hpp>
#include <cache/ObjResource.hpp>
#include "Texture.hpp"

namespace FearEngine::Render
{
class EditorModelLayer: public Layer
{
public:
	EditorModelLayer();

	errorCode init() override;

	void resize(int width, int height) override;
	void preUpdate(Component::Camera& cam) override;
	void update(Component::Camera& cam) override;
	void postUpdate(Component::Camera& cam) override;

	Render::debugProperty debugProperty() const override;

#pragma pack(push, 1)
	struct InstancedData
	{
		int entityNum;
		glm::mat4 transform;
	};
#pragma pack(pop, 1)
private:
	static constexpr const uint32_t MaxInstancedObjectCount = 10000;

	Render::VertexBuffer vertex;
	Render::VertexBuffer instanceBuffer;
	Render::Shaders::Shader shader;
	Render::VertexArray arr;

	Render::Shaders::Uniform projUniform;
	Render::Shaders::Uniform viewUniform;

	std::array<InstancedData, MaxInstancedObjectCount> instancedObjects;
};
}  // namespace FearEngine::Render

#endif
