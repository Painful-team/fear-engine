#ifndef FEARENGINE_RENDER_MODELLAYERS_H__
#define FEARENGINE_RENDER_MODELLAYERS_H__

#include <memory>

#include "Layer.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "shader/Shader.hpp"

#include <cache/ObjResource.hpp>
#include "Texture.hpp"
#include <cache/MaterialResource.hpp>

namespace FearEngine::Render
{
class ModelLayer: public Layer
{
public:
	ModelLayer();

	errorCode init() override;

	void resize(int width, int height) override;
	void preUpdate(Component::Camera& cam) override;
	void update(Component::Camera& cam) override;
	void postUpdate(Component::Camera& cam) override;
	uint32_t linkTexture(std::shared_ptr<Texture>& texture);
	uint32_t getEnabledTextureResource(std::shared_ptr<Cache::Resource>& resource);
	uint32_t getEnabledTexture(std::shared_ptr<Texture>& texture);
	Render::debugProperty debugProperty() const override;

private:
	Render::VertexBuffer vertex;
	Render::Shaders::Shader shader;
	Render::VertexArray arr;

	Render::Shaders::Uniform projUniform;
	Render::Shaders::Uniform viewUniform;
	Render::Shaders::Uniform modelUniform;

	Render::Shaders::Uniform entityIndex;
	Shaders::Shader::UniformStorage material;

	std::array<std::shared_ptr<Texture>, FearEngine::Render::Shaders::Shader::maxTextureSlots> textures;
	Cache::Material defaultMaterial;
	std::shared_ptr<Texture> emptyTexture;
	uint8_t enabledTextures;
};
}  // namespace FearEngine::Render

#endif
