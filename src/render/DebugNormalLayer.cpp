#include "DebugNormalLayer.hpp"

#include <core/Engine.hpp>

#include <components/MaterialComponent.hpp>
#include <utils/PointerCasts.hpp>

#include "Draws.hpp"

#include <vector>

namespace FearEngine::Render
{
DebugNormalsLayer::DebugNormalsLayer()
 : vertex({{Render::VertexBufferType::Float, 3}, {Render::VertexBufferType::Float, 3}, {Render::VertexBufferType::Float, 3},
	  {Render::VertexBufferType::Float, 2}})
{}

errorCode DebugNormalsLayer::init()
{
	auto result = shader.readShader("resources/shaders/normals/VertexLine.vert", Shaders::ShaderType::Vertex);
	if (result != errorCodes::OK)
	{
		return result;
	}

	result = shader.readShader("resources/shaders/normals/FragmentLine.frag", Shaders::ShaderType::Fragment);
	if (result != errorCodes::OK)
	{
		return result;
	}

	result = shader.readShader("resources/shaders/normals/Geometry.geom", Shaders::ShaderType::Geometry);
	if (result != errorCodes::OK)
	{
		return result;
	}

	result = shader.compile();
	if (result != errorCodes::OK)
	{
		return result;
	}

	shader.use();

	vertex.genBuffer();
	arr.genArray();

	arr.bind();

	vertex.bindData(100 * 1024 * 1024);

	arr.addVertexBuffer(vertex);

	vertex.unbind();
	arr.unBind();

	shader.use();

	color = shader.findUniform("color");

	projUniform = shader.findUniform("projection");
	viewUniform = shader.findUniform("view");
	modelUniform = shader.findUniform("model");

	color.setVec3(0.0, 0.5, 0.0);

	float magnitude = 0.2;
	shader.findUniform("magnitude").setFloat(&magnitude);

	return errorCodes::OK;
}

void DebugNormalsLayer::resize(int width, int height) {}

void DebugNormalsLayer::preUpdate(Component::Camera& cam) {}

void DebugNormalsLayer::update(Component::Camera& cam)
{
	shader.use();
	viewUniform.setMat4(&cam.getView());
	projUniform.setMat4(&cam.getProjection());

	uint32_t skipAttachment = 7;
	cam.beginView(&skipAttachment, 1);

	auto view = Engine::getScene()->view<Component::Renderable, Component::Transform>();
	for (auto entity : view)
	{
		auto& [renderable, tranform] = view.get<Component::Renderable, Component::Transform>(entity);
		modelUniform.setMat4(&tranform.getTransformMatrix());

		vertex.setData(renderable.mesh->data, renderable.mesh->size);
		shader.updateBuffers();

		auto extra = utils::reinterpret_pointer_cast<Cache::ObjData>(renderable.mesh->extra);
		Draws::draw(arr, extra->count);
	}

	cam.end();
}

void DebugNormalsLayer::postUpdate(Component::Camera& cam) {}

Render::debugProperty DebugNormalsLayer::debugProperty() const { return Render::debugProperties::Normals; }
}  // namespace FearEngine::Render
