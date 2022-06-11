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


#include <core/Scene.hpp>

namespace FearEngine::Render
{
ModelLayer::ModelLayer()
 : vertex({{Render::BufferType::Float, 3}, {Render::BufferType::Float, 3}, {Render::BufferType::Float, 3}, {Render::BufferType::Float, 2}})
 , enabledTextures(0)
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
	{
		auto texture = std::make_shared<Texture>();
		texture->init(model->materials.back()->diffuseRes);
		linkTexture(texture);
	}
	{
		auto texture = std::make_shared<Texture>();
		texture->init(model->materials.back()->specularRes);
		linkTexture(texture);
	}

	arr.addVertexBuffer(vertex);

	vertex.unbind();
	arr.unBind();

	shader.use();

	projUniform = shader.findUniform("projection");
	viewUniform = shader.findUniform("view");
	modelUniform = shader.findUniform("model");

	frame = shader.findUniform("wireframe");
	material = shader.findBuffer("Material");
	material["ambientStrength"].setVec3(model->materials.back()->ambient);
	material["shininess"].setFloat(model->materials.back()->shininess);
	material["diffuseTextureId"].setInt(0);
	material["specularTextureId"].setInt(1);

	modelUniform.setMat4(glm::mat4(1.0f));

	shader.findUniform("dirLight.dir").setVec3(-1.8, -1.8, -1);
	shader.findUniform("dirLight.lightColor").setVec3(1, 1, 1);

	int32_t samplers[Shaders::Shader::maxTextureSlots];
	for (uint8_t i = 0; i < Shaders::Shader::maxTextureSlots; ++i)
	{
		samplers[i] = i;
	}

	shader.findUniform("textures").setInt(samplers, Shaders::Shader::maxTextureSlots);
}

void ModelLayer::resize(int width, int height) 
{}

void ModelLayer::preUpdate(Component::Camera& cam)
{
	arr.bind();
	cam.setUniforms(projUniform, viewUniform);
	cam.beginView();
}

void ModelLayer::update(Component::Camera& cam)
{

	//frame.setFloat(1);
	//
	//shader.updateBuffers();
	//
	//// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glPolygonOffset(1, 0.1);
	//glDrawArrays(GL_TRIANGLES, 0, model->vertices.size());
	// glDrawElements(GL_TRIANGLES, chunk.triangles.size(), GL_UNSIGNED_INT, 0);
	
	frame.setFloat(0);

	modelUniform.setMat4(glm::mat4(1.0f));

	for (uint8_t i = 0; i < enabledTextures; ++i)
	{
		textures[i]->enable(i);
	}

	shader.updateBuffers();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glPolygonOffset(1, 0);
	// glDrawElements(GL_TRIANGLES, chunk.triangles.size(), GL_UNSIGNED_INT, 0);

	glDrawArrays(GL_TRIANGLES, 0, model->vertices.size());
}

void ModelLayer::postUpdate(Component::Camera& cam)
{
	glBindVertexArray(0);
	cam.end();
}

void ModelLayer::linkTexture(std::shared_ptr<Texture>& texture)
{
	textures[enabledTextures] = texture;
	++enabledTextures;
}
}  // namespace FearEngine::Render
