#include "LightPass.hpp"

#include <core/Engine.hpp>

#include <render/Draws.hpp>

#include <components/MaterialComponent.hpp>

FearEngine::Render::LightPass::LightPass() {}

FearEngine::Render::errorCode FearEngine::Render::LightPass::init()
{
	auto result = shader.readShader("resources/shaders/light/lightVert.vert", Shaders::ShaderType::Vertex);
	if (result != errorCodes::OK)
	{
		return result;
	}

	result = shader.readShader("resources/shaders/light/lightFrag.frag", Shaders::ShaderType::Fragment);
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

	projUniform = shader.findUniform("projection");
	viewUniform = shader.findUniform("view");

	dir = shader.findUniform("dir");
	lightColor = shader.findUniform("lightColor");
	dirLightCount = shader.findUniform("dirLightCount");

	shader.findUniform("geometry").setInt(1);
	shader.findUniform("normals").setInt(2);
	shader.findUniform("albedo").setInt(3);
	shader.findUniform("ambients").setInt(4);
	shader.findUniform("lights").setInt(10);
	lightCountUniform = shader.findUniform("lightCount");

	lightData.genBuffer();
	VertexBuffer tempScreenPosBuf{{VertexBufferType::Float, 3}, {VertexBufferType::Float, 2}};
	tempScreenPosBuf.genBuffer();

	std::array texPos{
		 // positions        // texture Coords
		 -1.0f,
		 1.0f,
		 0.0f,
		 0.0f,
		 1.0f,
		 -1.0f,
		 -1.0f,
		 0.0f,
		 0.0f,
		 0.0f,
		 1.0f,
		 1.0f,
		 0.0f,
		 1.0f,
		 1.0f,
		 1.0f,
		 -1.0f,
		 0.0f,
		 1.0f,
		 0.0f,

	};

	arr.genArray();

	arr.bind();
	tempScreenPosBuf.bindData(texPos.data(), texPos.size() * sizeof(texPos.data()));
	arr.addVertexBuffer(tempScreenPosBuf);

	tempScreenPosBuf.unbind();
	lightData.unbind();
	arr.unBind();

	shader.use();

	lightData.genBuffer();

	lightData.reserveData(lights.size() * sizeof(Light));

	return errorCodes::OK;
}

void FearEngine::Render::LightPass::resize(int width, int height) {}

void FearEngine::Render::LightPass::preUpdate(Component::Camera& cam) {}

void FearEngine::Render::LightPass::update(Component::Camera& cam)
{
	shader.use();
	viewUniform.setMat4(&cam.getView());
	projUniform.setMat4(&cam.getProjection());

	uint32_t skipAttachment = 7;
	cam.beginView(&skipAttachment, 1);

	glBindTextureUnit(1, cam.getFrameBuffer().getColorAttachment(1));
	glBindTextureUnit(2, cam.getFrameBuffer().getColorAttachment(2));
	glBindTextureUnit(3, cam.getFrameBuffer().getColorAttachment(3));
	glBindTextureUnit(4, cam.getFrameBuffer().getColorAttachment(4));

	{
		//auto view = Engine::getScene()->view<Component::DirectionalLight>();
		//int lightCount = 0;
		//for (auto entity : view)
		//{
		//	auto& dirl = view.get<Component::DirectionalLight>(entity);
		//	dir.setVec3(&dirl.dir, 1, sizeof(glm::vec3) * lightCount);
		//	lightColor.setVec3(&dirl.lightColor, 1, sizeof(glm::vec3) * lightCount);
		//	++lightCount;
		//}
		//
		//dirLightCount.setInt(&lightCount);
	}

	{
		auto view = Engine::getScene()->view<Component::Light, Component::Transform>();
		uint32_t lightCount = 0;
		for (auto entity : view)
		{
			auto& [light, tranform] = view.get<Component::Light, Component::Transform>(entity);

			lights[lightCount] = light;
			lights[lightCount].pos = tranform.pos;

			++lightCount;
		}

		lightCountUniform.setInt(lightCount);
		lightData.setData(lights.data(), lightCount * sizeof(Light));

		shader.updateBuffers();

		assert(lightCount < maxLightCount);
		Draws::drawStrip(arr, 4);
	}

	cam.end();
}

void FearEngine::Render::LightPass::postUpdate(Component::Camera& cam) {}

FearEngine::Render::debugProperty FearEngine::Render::LightPass::debugProperty() const { return Render::debugProperties::None; }

FearEngine::Render::LightPass::Light& FearEngine::Render::LightPass::Light::operator=(const Component::Light& light)
{
	isPoint = light.isPoint;
	dir = light.dir;

	distance = light.distance;

	cutOff = light.cutOff;
	outerCutOff = light.outerCutOff;

	lightColor = light.lightColor;

	return *this;
}
