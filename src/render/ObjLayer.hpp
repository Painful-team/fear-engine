#ifndef FEARENGINE_RENDER_OBJLAYER_H__
#define FEARENGINE_RENDER_OBJLAYER_H__

#include "Layer.hpp"
#include "VertexBuffer.hpp"
#include "VertexArray.hpp"
#include "shader/Shader.hpp"

#include "Camera.hpp"

namespace FearEngine::Render
{
class ModelLayer: public Layer
{
public:
	ModelLayer();

	void init() override;
	void update() override;

private:
	Render::VertexBuffer vertex;
	Render::Shaders::Shader shader;
	Render::VertexArray arr;
	std::vector<Vertex> vertices;

	Render::Shaders::Uniform projUniform;
	Render::Shaders::Uniform viewUniform;
	Render::Shaders::Uniform modelUniform;
	Render::Shaders::Uniform frame;

	Render::Camera camera;
};
}

#endif
