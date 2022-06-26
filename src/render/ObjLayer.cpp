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

#include <components/MaterialComponent.hpp>

#include <core/Scene.hpp>
#include <core/Entity.hpp>

namespace FearEngine::Render
{
ModelLayer::ModelLayer()
 : vertex({{Render::BufferType::Float, 3}, {Render::BufferType::Float, 3}, {Render::BufferType::Float, 3}, {Render::BufferType::Float, 2}})
 , enabledTextures(0)
{}

errorCode ModelLayer::init()
{
	auto result = shader.readShader("resources/shaders/Vertex.vert", Shaders::ShaderType::Vertex);
	if (result != errorCodes::OK)
	{
		return result;
	}

	result = shader.readShader("resources/shaders/Fragment.frag", Shaders::ShaderType::Fragment);
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

	projUniform = shader.findUniform("projection");
	viewUniform = shader.findUniform("view");
	modelUniform = shader.findUniform("model");

	frame = shader.findUniform("wireframe");
	entityIndex = shader.findUniform("entityIndex");
	frame.setFloat(0);

	material = shader.findBuffer("Material");

	shader.findUniform("dirLight.dir").setVec3(-1.8, -1.8, -1);
	shader.findUniform("dirLight.lightColor").setVec3(1, 1, 1);

	int32_t samplers[Shaders::Shader::maxTextureSlots];
	for (uint8_t i = 0; i < Shaders::Shader::maxTextureSlots; ++i)
	{
		//Todo disable warning about empty sampler with enabling empty textures
		samplers[i] = i;
	}

	shader.findUniform("textures").setInt(samplers, Shaders::Shader::maxTextureSlots);
}

void ModelLayer::resize(int width, int height)
{}

void ModelLayer::preUpdate(Component::Camera& cam)
{
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
	arr.bind();

	auto& view = Engine::getScene()->view<Component::Renderable, Component::Transform>();
	for (auto& entity: view)
	{
		auto& [renderable, tranform] = view.get<Component::Renderable, Component::Transform>(entity);
		modelUniform.setMat4(tranform.getTransformMatrix());
		//entityIndex.setInt((uint32_t)entity);
		{
			uint32_t unit = getEnabledTexture(renderable.materials.back()->diffuseRes);
			if (unit == -1)
			{
				auto texture = std::make_shared<Texture>();
				texture->init(renderable.materials.back()->diffuseRes);
				unit = linkTexture(texture);
			}
			material["diffuseTextureId"].setInt(unit);
		}

		{
			uint32_t unit = getEnabledTexture(renderable.materials.back()->specularRes);
			if (unit == -1)
			{
				auto texture = std::make_shared<Texture>();
				texture->init(renderable.materials.back()->specularRes);
				unit = linkTexture(texture);
			}
			material["specularTextureId"].setInt(unit);
		}

		material["ambientStrength"].setVec3(renderable.materials.back()->ambient);
		material["shininess"].setFloat(renderable.materials.back()->shininess);

		vertex.bindData(reinterpret_cast<float*>(renderable.mesh->data), renderable.mesh->size);
		for (uint8_t i = 0; i < enabledTextures; ++i)
		{
			textures[i]->enable(i);
		}

		shader.updateBuffers();

		// glDrawElements(GL_TRIANGLES, chunk.triangles.size(), GL_UNSIGNED_INT, 0);

		glDrawArrays(GL_TRIANGLES, 0, renderable.mesh->size / sizeof(float));
	}
}

void ModelLayer::postUpdate(Component::Camera& cam)
{
	arr.unBind();
	cam.end();
}

uint32_t ModelLayer::linkTexture(std::shared_ptr<Texture>& texture)
{
	textures[enabledTextures] = texture;

	uint8_t prev = enabledTextures;
	enabledTextures = enabledTextures + 1 % textures.size();
	return prev;
}

uint32_t ModelLayer::getEnabledTexture(std::shared_ptr<Cache::Resource>& resource)
{
	for (uint8_t i = 0; i < textures.size() && textures[i]; ++i)
	{
		if (textures[i]->getResource().get() == resource.get())
		{
			return i;
		}
	}

	return -1;
}

}  // namespace FearEngine::Render
