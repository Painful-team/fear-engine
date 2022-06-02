#ifndef FEARENGINE_RENDER_OBJLAYER_H__
#define FEARENGINE_RENDER_OBJLAYER_H__

#include "Layer.hpp"
#include "VertexArray.hpp"
#include "VertexBuffer.hpp"
#include "shader/Shader.hpp"

#include "Camera.hpp"

#include <cache/ObjResource.hpp>

namespace FearEngine::Render
{
class ModelLayer: public Layer
{
public:
	ModelLayer();

	void init() override;

	void resize(int width, int height) override;
	void preUpdate() override;
	void update() override;
	void postUpdate() override;

private:
	Render::VertexBuffer vertex;
	Render::Shaders::Shader shader;
	Render::VertexArray arr;

	std::shared_ptr<Cache::ObjData> model;

	Render::Shaders::Uniform projUniform;
	Render::Shaders::Uniform viewUniform;
	Render::Shaders::Uniform modelUniform;
	Render::Shaders::Uniform frame;

	Render::Camera camera;

};
}  // namespace FearEngine::Render

#endif
