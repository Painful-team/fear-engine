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

	projUniform.init(shader.getId(), "projection");
	viewUniform.init(shader.getId(), "view");
	modelUniform.init(shader.getId(), "model");

	frame.init(shader.getId(), "inframe");

	this->camera = Camera(projUniform);
	camera.setProjection(false);

	shader.use();
}

void ModelLayer::update()
{
	static glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	static glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	static glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	if (Input::isKeyPressed(Events::Key::W))
		cameraPos += camera.getSpeed() * cameraFront;
	if (Input::isKeyPressed(Events::Key::S))
		cameraPos -= camera.getSpeed() * cameraFront;
	if (Input::isKeyPressed(Events::Key::A))
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * camera.getSpeed();
	if (Input::isKeyPressed(Events::Key::D))
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * camera.getSpeed();

	glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

	viewUniform.setMat4(view);

	modelUniform.setMat4(glm::mat4(1.0f));

	arr.bind();
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	frame.setFloat(1);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glPolygonOffset(1, 0.1);
	glDrawArrays(GL_TRIANGLES, 0, model->vertices.size());
	// glDrawElements(GL_TRIANGLES, chunk.triangles.size(), GL_UNSIGNED_INT, 0);

	frame.setFloat(0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glPolygonOffset(1, 0);
	// glDrawElements(GL_TRIANGLES, chunk.triangles.size(), GL_UNSIGNED_INT, 0);

	glDrawArrays(GL_TRIANGLES, 0, model->vertices.size());

	glBindVertexArray(0);
}
}  // namespace FearEngine::Render
