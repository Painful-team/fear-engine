#ifndef FEARENGINE_RENDER_DEBUGNORMALLAYER_H__
#define FEARENGINE_RENDER_DEBUGNORMALLAYER_H__

#include <memory>

#include "Layer.hpp"
#include "VertexArray.hpp"
#include "Buffer.hpp"
#include "shader/Shader.hpp"

#include <cache/ObjResource.hpp>

namespace FearEngine::Render
{
class DebugNormalsLayer: public Layer
{
public:
	DebugNormalsLayer();

	errorCode init() override;

	void resize(int width, int height) override;
	void preUpdate(Component::Camera& cam) override;
	void update(Component::Camera& cam) override;
	void postUpdate(Component::Camera& cam) override;
	Render::debugProperty debugProperty() const override;

private:
	Render::Buffer vertex;
	Render::Shaders::Shader shader;
	Render::VertexArray arr;

	Render::Shaders::Uniform projUniform;
	Render::Shaders::Uniform viewUniform;
	Render::Shaders::Uniform modelUniform;

	Render::Shaders::Uniform color;

};
}	// namespace FearEngine::Render
#endif
