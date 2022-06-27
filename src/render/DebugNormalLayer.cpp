#include "DebugNormalLayer.hpp"

#include <core/Engine.hpp>

#include <components/MaterialComponent.hpp>

#include <vector>

namespace FearEngine::Render
{
DebugNormalsLayer::DebugNormalsLayer()
 : vertex({{Render::BufferType::Float, 3}, {Render::BufferType::Float, 3}, {Render::BufferType::Float, 3}, {Render::BufferType::Float, 2}})
{}

errorCode DebugNormalsLayer::init()
{
	auto result = shader.readShader("resources/shaders/VertexLine.vert", Shaders::ShaderType::Vertex);
	if (result != errorCodes::OK)
	{
		return result;
	}

	result = shader.readShader("resources/shaders/FragmentLine.frag", Shaders::ShaderType::Fragment);
	if (result != errorCodes::OK)
	{
		return result;
	}

	result = shader.readShader("resources/shaders/Geometry.geom", Shaders::ShaderType::Geometry);
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

	color.setVec3(0, 0.5, 0);

	shader.findUniform("magnitude").setFloat(0.2);

	return errorCodes::OK;
}

void DebugNormalsLayer::resize(int width, int height) {}

void DebugNormalsLayer::preUpdate(Component::Camera& cam)
{
	shader.use();
	cam.setUniforms(projUniform, viewUniform);
	cam.beginView();

	arr.bind();
}

void DebugNormalsLayer::update(Component::Camera& cam)
{
	auto view = Engine::getScene()->view<Component::Renderable, Component::Transform>();
	for (auto entity : view)
	{
		auto& [renderable, tranform] = view.get<Component::Renderable, Component::Transform>(entity);
		modelUniform.setMat4(tranform.getTransformMatrix());

		vertex.setData(reinterpret_cast<float*>(renderable.mesh->data), renderable.mesh->size);
		shader.updateBuffers();

		// glDrawElements(GL_TRIANGLES, chunk.triangles.size(), GL_UNSIGNED_INT, 0);

		glDrawArrays(GL_TRIANGLES, 0, renderable.mesh->size / sizeof(float));
	}
}

void DebugNormalsLayer::postUpdate(Component::Camera& cam)
{
	arr.unBind();
	cam.end();
}
Render::debugProperty DebugNormalsLayer::debugProperty() const { return Render::debugProperties::Normals; }
}  // namespace FearEngine::Render
