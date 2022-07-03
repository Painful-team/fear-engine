#include "EditorModelLayer.hpp"

#include <render/Renderer.hpp>
#include <core/Engine.hpp>

#include <cache/Resource.hpp>

#include <components/CameraComponent.hpp>
#include <components/MaterialComponent.hpp>

#include <utils/PointerCasts.hpp>

#include "Draws.hpp"

FearEngine::Render::EditorModelLayer::EditorModelLayer()
 : vertex({{Render::BufferType::Float, 3}, {Render::BufferType::Float, 3}, {Render::BufferType::Float, 3}, {Render::BufferType::Float, 2}})
 , instanceBuffer({{Render::BufferType::Int, 1}, {Render::BufferType::Float, 4}, {Render::BufferType::Float, 4},
		{Render::BufferType::Float, 4}, {Render::BufferType::Float, 4}})
{}

FearEngine::Render::errorCode FearEngine::Render::EditorModelLayer::init()
{
	auto result = shader.readShader("resources/shaders/editor/EditorObjects.vert", Shaders::ShaderType::Vertex);
	if (result != errorCodes::OK)
	{
		return result;
	}

	result = shader.readShader("resources/shaders/editor/EditorObjects.frag", Shaders::ShaderType::Fragment);
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

	instanceBuffer.genBuffer();
	instanceBuffer.bindData(sizeof(InstancedData) * MaxInstancedObjectCount);
	arr.addVertexBuffer(instanceBuffer, VertexArrayUpdateTypes::OnDraw);

	vertex.unbind();
	instanceBuffer.unbind();
	arr.unBind();

	shader.use();

	projUniform = shader.findUniform("projection");
	viewUniform = shader.findUniform("view");

	shader.findUniform("color").setVec3(0.3, 0.3, 0.3);
	shader.findUniform("disableTexture").setInt(1);
	shader.findUniform("textureId").setInt(0);

	return errorCodes::OK;
}

void FearEngine::Render::EditorModelLayer::resize(int width, int height) {}

void FearEngine::Render::EditorModelLayer::preUpdate(Component::Camera& cam) {}

void FearEngine::Render::EditorModelLayer::update(Component::Camera& cam)
{
	shader.use();
	viewUniform.setMat4(cam.getView());
	projUniform.setMat4(cam.getProjection());
	cam.beginView();

	{
		static std::shared_ptr<Cache::Resource> resource;
		static auto loadedModel = Engine::getCache()->getResource("resources/models/editor/Camera.obj", resource);
		assert(loadedModel == Render::errorCodes::OK && "Default model for editor not found.");

		auto extra = utils::reinterpret_pointer_cast<Cache::ObjData>(resource->extra);
		auto view = Engine::getScene()->view<Component::Camera, Component::Transform>();
		uint32_t size = view.size_hint();
		uint32_t i = 0;
		for (auto entity : view)
		{
			if (i >= size)
			{
				break;
			}

			auto trans = view.get<Component::Transform>(entity);
			auto& camData = view.get<Component::Camera>(entity);

			trans.rotation = -trans.rotation;
			std::swap(trans.rotation.x, trans.rotation.z);
			instancedObjects[i].transform = trans.getTransformMatrix();
			instancedObjects[i].entityNum = entity;

			float fov = camData.getFOV();
			float far = camData.getFar();
			float near = camData.getNear();

			float tan2 = tan(fov / 2);
			glm::vec3 right = glm::vec3(0, 0, 1);
			glm::vec3 up = glm::vec3(0, 1, 0);
			glm::vec3 forward = glm::vec3(1, 0, 0);

			glm::vec3 nearRight = near * tan2 * right * camData.getAspect();
			glm::vec3 farRight = far * tan2 * right * camData.getAspect();
			glm::vec3 nearUp = near * tan2 * up;
			glm::vec3 farUp = far * tan2 * up;

			glm::mat4& tranform = instancedObjects[i].transform;

			Draws::OutlineVertice nearPlane[4];
			nearPlane[0].pos = tranform * glm::vec4(near * forward - nearRight + nearUp, 1.0);
			nearPlane[1].pos = tranform * glm::vec4(near * forward + nearRight + nearUp, 1.0);
			nearPlane[2].pos = tranform * glm::vec4(near * forward + nearRight - nearUp, 1.0);
			nearPlane[3].pos = tranform * glm::vec4(near * forward - nearRight - nearUp, 1.0);

			Draws::OutlineVertice farPlane[4];
			farPlane[0].pos = tranform * glm::vec4(far * forward - farRight + farUp, 1.0);
			farPlane[1].pos = tranform * glm::vec4(far * forward + farRight + farUp, 1.0);
			farPlane[2].pos = tranform * glm::vec4(far * forward + farRight - farUp, 1.0);
			farPlane[3].pos = tranform * glm::vec4(far * forward - farRight - farUp, 1.0);

			for (uint8_t i = 0; i < 4; ++i)
			{
				nearPlane[i].color = glm::vec3(0, 1.0f, 0);
				farPlane[i].color = glm::vec3(1.0f, 0, 0);
			}

			Draws::OutlineVertice leftPlane[4];
			leftPlane[0] = nearPlane[0];
			leftPlane[1] = farPlane[0];
			leftPlane[2] = nearPlane[3];
			leftPlane[3] = farPlane[3];

			Draws::OutlineVertice rightPlane[4];
			rightPlane[0] = nearPlane[1];
			rightPlane[1] = farPlane[1];
			rightPlane[2] = nearPlane[2];
			rightPlane[3] = farPlane[2];

			Draws::drawOutlines(nearPlane, 4);
			Draws::drawOutlines(farPlane, 4);
			Draws::drawOutlines(leftPlane, 4);
			Draws::drawOutlines(rightPlane, 4);
			++i;
		}

		shader.updateBuffers();

		arr.bind();
		vertex.setData(reinterpret_cast<float*>(resource->data), resource->size);
		instanceBuffer.setData(reinterpret_cast<float*>(instancedObjects.data()), sizeof(InstancedData) * i);

		Draws::drawIndexed(arr, extra->count, i);
	}

	cam.end();
}

void FearEngine::Render::EditorModelLayer::postUpdate(Component::Camera& cam) {}

FearEngine::Render::debugProperty FearEngine::Render::EditorModelLayer::debugProperty() const
{
	return Render::debugProperties::SceneObjects;
}
