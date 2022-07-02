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
 , instanceBuffer(
		{{Render::BufferType::Float, 4}, {Render::BufferType::Float, 4}, {Render::BufferType::Float, 4}, {Render::BufferType::Float, 4}, {Render::BufferType::Int, 1}})
{
}

FearEngine::Render::errorCode FearEngine::Render::EditorModelLayer::init() { 
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

void FearEngine::Render::EditorModelLayer::preUpdate(Component::Camera& cam)
{
	shader.use();
	viewUniform.setMat4(cam.getView());
	projUniform.setMat4(cam.getProjection());
	cam.beginView();
}
void FearEngine::Render::EditorModelLayer::update(Component::Camera& cam)
{
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

			float fov = cam.getFOV();
			float far = cam.getFar();
			float near = camData.getNear();

			float tan2 = tan(fov / 2);
			glm::vec3 right = glm::vec3(1, 0, 0);
			glm::vec3 up = glm::vec3(0, 1, 0);
			glm::vec3 forward = glm::vec3(0, 0, 1);

			glm::vec3 nearRight = near * tan2 * right * cam.getAspect();
			glm::vec3 farRight = far * tan2 * right * cam.getAspect();
			glm::vec3 nearUp = near * tan2 * up * cam.getAspect();
			glm::vec3 farUp = far * tan2 * up * cam.getAspect();


			glm::vec4 nearPlane[4];
			nearPlane[0] = trans.getTransformMatrix() * glm::vec4(near * forward - nearRight + nearUp, 1.0);
			nearPlane[1] = trans.getTransformMatrix() * glm::vec4(near * forward + nearRight + nearUp, 1.0);
			nearPlane[1] = trans.getTransformMatrix() * glm::vec4(near * forward + nearRight - nearUp, 1.0);
			nearPlane[1] = trans.getTransformMatrix() * glm::vec4(near * forward - nearRight - nearUp, 1.0);

			glm::vec4 farPlane[4];
			farPlane[0] = trans.getTransformMatrix() * glm::vec4(far * forward - farRight + farUp, 1.0);
			farPlane[1] = trans.getTransformMatrix() * glm::vec4(far * forward + farRight + farUp, 1.0);
			farPlane[1] = trans.getTransformMatrix() * glm::vec4(far * forward + farRight - farUp, 1.0);
			farPlane[1] = trans.getTransformMatrix() * glm::vec4(far * forward - farRight - farUp, 1.0);

			std::cout << nearPlane[0].x << ' ' << nearPlane[0].y << ' ' << nearPlane[0].z << "----\n";
			std::cout << '\t' << farPlane[0].x << ' ' << farPlane[0].y << ' ' << farPlane[0].z << std::endl;

			instancedObjects[i].transform = trans.getTransformMatrix();
			instancedObjects[i].entityNum = entity;
			++i;
		}

		shader.updateBuffers();

		vertex.setData(reinterpret_cast<float*>(resource->data), resource->size);
		instanceBuffer.setData(reinterpret_cast<float*>(instancedObjects.data()), sizeof(InstancedData) * i);

		Draws::drawIndexed(arr, extra->count, i);
	}
}

void FearEngine::Render::EditorModelLayer::postUpdate(Component::Camera& cam)
{
	cam.end();
}

FearEngine::Render::debugProperty FearEngine::Render::EditorModelLayer::debugProperty() const
{
	return Render::debugProperties::SceneObjects;
}
