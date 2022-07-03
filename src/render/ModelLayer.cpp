#include <glad/glad.h>
#include "ModelLayer.hpp"

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

#include "Draws.hpp"

namespace FearEngine::Render
{
ModelLayer::ModelLayer()
 : vertex({{Render::BufferType::Float, 3}, {Render::BufferType::Float, 3}, {Render::BufferType::Float, 3}, {Render::BufferType::Float, 2}})
 , enabledTextures(0)
{}

errorCode ModelLayer::init()
{
	auto result = shader.readShader("resources/shaders/objects/Vertex.vert", Shaders::ShaderType::Vertex);
	if (result != errorCodes::OK)
	{
		return result;
	}

	result = shader.readShader("resources/shaders/objects/Fragment.frag", Shaders::ShaderType::Fragment);
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

	// TODO Get Texture Slots from GPU
	int32_t samplers[Shaders::Shader::maxTextureSlots];
	for (uint8_t i = 0; i < Shaders::Shader::maxTextureSlots; ++i)
	{
		samplers[i] = i;
	}

	shader.findUniform("textures").setInt(samplers, Shaders::Shader::maxTextureSlots);

	emptyTexture = std::make_shared<Texture>();
	uint8_t texColor[] = {128, 128, 128};
	emptyTexture->initEmpty(texColor);
	for (uint32_t i = 0; i < textures.size(); ++i)
	{
		textures[i] = emptyTexture;
	}

	defaultMaterial.ambient = glm::vec3(1);
	defaultMaterial.diffuse = glm::vec3(0.8);
	defaultMaterial.shininess = 32;

	return Render::errorCodes::OK;
}

void ModelLayer::resize(int width, int height) {}

void ModelLayer::preUpdate(Component::Camera& cam) {}

void ModelLayer::update(Component::Camera& cam)
{
	shader.use();
	viewUniform.setMat4(cam.getView());
	projUniform.setMat4(cam.getProjection());
	cam.beginView();

	auto& view = Engine::getScene()->view<Component::Renderable, Component::Transform>();
	for (auto entity : view)
	{
		auto& [renderable, tranform] = view.get<Component::Renderable, Component::Transform>(entity);
		modelUniform.setMat4(tranform.getTransformMatrix());
		entityIndex.setInt(entity);

		if (!renderable.materials.empty())
		{
			auto& materialData = renderable.materials.back();
			{
				uint32_t unit = getEnabledTextureResource(materialData->diffuseRes);
				if (unit == -1)
				{
					auto texture = std::make_shared<Texture>();
					texture->init(renderable.materials.back()->diffuseRes);
					unit = linkTexture(texture);
				}
				material["diffuseTextureId"].setInt(unit);
			}

			{
				uint32_t unit = getEnabledTextureResource(materialData->specularRes);
				if (unit == -1)
				{
					auto texture = std::make_shared<Texture>();
					texture->init(materialData->specularRes);
					unit = linkTexture(texture);
				}
				material["specularTextureId"].setInt(unit);
			}

			material["ambientStrength"].setVec3(materialData->ambient);
			material["shininess"].setFloat(materialData->shininess);
		}
		else
		{
			uint32_t unit = getEnabledTexture(emptyTexture);
			material["diffuseTextureId"].setInt(unit);
			material["specularTextureId"].setInt(unit);

			material["ambientStrength"].setVec3(defaultMaterial.ambient);
			material["shininess"].setFloat(defaultMaterial.shininess);
		}

		vertex.setData(reinterpret_cast<float*>(renderable.mesh->data), renderable.mesh->size);
		for (uint8_t i = 0; i < textures.size(); ++i)
		{
			textures[i]->enable(i);
		}

		shader.updateBuffers();

		// glDrawElements(GL_TRIANGLES, chunk.triangles.size(), GL_UNSIGNED_INT, 0);

		auto extra = utils::reinterpret_pointer_cast<Cache::ObjData>(renderable.mesh->extra);
		Draws::draw(arr, extra->count);
	}

	cam.end();
}

void ModelLayer::postUpdate(Component::Camera& cam) {}

uint32_t ModelLayer::linkTexture(std::shared_ptr<Texture>& texture)
{
	textures[enabledTextures] = texture;

	uint8_t prev = enabledTextures;
	enabledTextures = enabledTextures + 1 % textures.size();
	return prev;
}

uint32_t ModelLayer::getEnabledTextureResource(std::shared_ptr<Cache::Resource>& resource)
{
	for (uint8_t i = 0; i < textures.size(); ++i)
	{
		if (textures[i]->getResource().get() == resource.get())
		{
			return i;
		}
	}

	return -1;
}

uint32_t ModelLayer::getEnabledTexture(std::shared_ptr<Texture>& texture)
{
	for (uint8_t i = 0; i < textures.size(); ++i)
	{
		if (textures[i]->getTexHandle() == texture->getTexHandle())
		{
			return i;
		}
	}

	return -1;
}

debugProperty ModelLayer::debugProperty() const { return debugProperties::None; }

}  // namespace FearEngine::Render
