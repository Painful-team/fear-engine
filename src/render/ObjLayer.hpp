#ifndef FEARENGINE_RENDER_OBJLAYER_H__
#define FEARENGINE_RENDER_OBJLAYER_H__

#include <memory>

#include "Layer.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "shader/Shader.hpp"

#include <cache/ObjResource.hpp>
#include "Texture.hpp"

namespace FearEngine::Render
{
class ModelLayer: public Layer
{
public:
	ModelLayer();

	void init() override;

	void resize(int width, int height) override;
	void preUpdate(Component::Camera& cam) override;
	void update(Component::Camera& cam) override;
	void postUpdate(Component::Camera& cam) override;
	void linkTexture(std::shared_ptr<Texture>& texture);

private:
	Render::VertexBuffer vertex;
	Render::Shaders::Shader shader;
	Render::VertexArray arr;

	std::shared_ptr<Cache::ObjData> model;

	Render::Shaders::Uniform projUniform;
	Render::Shaders::Uniform viewUniform;
	Render::Shaders::Uniform modelUniform;

	Render::Shaders::Uniform frame;
	Shaders::Shader::UniformStorage material;

	std::array<std::shared_ptr<Texture>, FearEngine::Render::Shaders::Shader::maxTextureSlots> textures;

	uint8_t enabledTextures;
};
}  // namespace FearEngine::Render

#endif
