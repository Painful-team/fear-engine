#include <glad/glad.h>
#include "ObjLayer.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <core/Engine.hpp>
#include <core/Input.hpp>

#include "shader/Uniform.hpp"

#include <cache/ObjResource.hpp>
#include <utils/PointerCasts.hpp>

namespace FearEngine::Render
{
ModelLayer::ModelLayer()
 : vertex({{Render::BufferType::Float, 3}, {Render::BufferType::Float, 3}, {Render::BufferType::Float, 3}})
{}

void ModelLayer::init()
{
	shader.readShader("resources/shaders/Vertex.vert", GL_VERTEX_SHADER);
	shader.readShader("resources/shaders/Fragment.frag", GL_FRAGMENT_SHADER);
	shader.compile();
	shader.use();

	vertex.genBuffer();
	arr.genArray();

	arr.bind();

	std::shared_ptr<Cache::Resource> resource;
	auto result = Engine::getCache()->getResource("resources/models/backpack.obj", resource);
	model = utils::static_pointer_cast<Cache::ObjData>(resource->extra);

	vertex.bindData((float*)resource->data, resource->size);

	arr.addVertexBuffer(vertex);

	vertex.unbind();
	arr.unBind();

	shader.use();

	projUniform = shader.findUniform("projection");
	viewUniform = shader.findUniform("view");
	modelUniform = shader.findUniform("model");

	frame = shader.findUniform("wireframe");

	this->camera = Camera(projUniform, viewUniform);
	camera.init();

	shader.use();

	modelUniform.setMat4(glm::mat4(1.0f));
}

void ModelLayer::update()
{
  arr.bind();
	frame.setFloat(1);

	shader.updateBuffers();
  
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonOffset(1, 0.1);
	glDrawArrays(GL_TRIANGLES, 0, model->vertices.size());
	// glDrawElements(GL_TRIANGLES, chunk.triangles.size(), GL_UNSIGNED_INT, 0);

	frame.setFloat(0);

	viewUniform.setMat4(view);

	modelUniform.setMat4(glm::mat4(1.0f));

	shader.updateBuffers();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glPolygonOffset(1, 0);
	// glDrawElements(GL_TRIANGLES, chunk.triangles.size(), GL_UNSIGNED_INT, 0);

	glDrawArrays(GL_TRIANGLES, 0, model->vertices.size());

	glBindVertexArray(0);
}
}  // namespace FearEngine::Render
