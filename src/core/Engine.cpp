#include "Engine.hpp"
#include "glad/glad.h"

#include <cassert>
#include <iostream>
#include <memory>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <event/KeyCodes.hpp>
#include <event/detail/Delegate.hpp>
#include <render/shader/Uniform.hpp>

#include "Input.hpp"
#include "Window.hpp"
#include "events.hpp"

#include <cache/CacheManager.hpp>

#include "Entity.hpp"
#include "Logger.hpp"

#include <editor/Editor.hpp>

#include <utils/PointerCasts.hpp>
#include <components/MaterialComponent.hpp>
#include <cache/ObjResource.hpp>

namespace FearEngine
{
std::unique_ptr<Logger> Engine::loggers;
std::unique_ptr<Events::Dispatcher> Engine::eventDispatcher;
std::unique_ptr<CacheManager> Engine::cacheManager;
std::unique_ptr<Window> Engine::window;
std::unique_ptr<Editor> Engine::editor;
std::unique_ptr<Scene> Engine::scene;
std::unique_ptr<Renderer> Engine::renderer;

void Engine::onEvent(Events::Event* event)
{
	loggers->log("Event", "{0} occurred.", event->name());
	eventDispatcher->notify(event);
}

bool Engine::onMinimized(Events::WindowMinimized* event)
{
	minimized = true;
	return true;
}

bool Engine::onRestore(Events::WindowRestored* event)
{
	minimized = false;
	return true;
}

bool Engine::onResize(Events::WindowResize* event)
{
	if (event->getWidth() == 0 || event->getHeight() == 0)
	{
		minimized = true;
		return true;
	}

	minimized = false;
	editor->resize(event->getWidth(), event->getHeight());
	renderer->onResize(event);

	return true;
}

bool Engine::onClose(Events::WindowClose* event)
{
	running = false;
	return true;
}

std::unique_ptr<Renderer>& Engine::getRender() { return Engine::renderer; }

std::unique_ptr<Window>& Engine::getWindow() { return Engine::window; }

std::unique_ptr<Events::Dispatcher>& Engine::getDispatcher() { return Engine::eventDispatcher; }

std::unique_ptr<CacheManager>& Engine::getCache() { return Engine::cacheManager; }

std::unique_ptr<Editor>& Engine::getEditor() { return Engine::editor; }

std::unique_ptr<Scene>& Engine::getScene() { return Engine::scene; }

std::unique_ptr<Logger>& Engine::logs() { return Engine::loggers; }

int Engine::init()
{
	Engine::loggers = std::make_unique<Logger>();
	auto result = loggers->init();
	assert(result == 0);

	Engine::eventDispatcher = std::make_unique<Events::Dispatcher>();
	eventDispatcher->get<Events::WindowRestored>()->attach<&Engine::onRestore>(this);

	eventDispatcher->get<Events::WindowResize>()->attach<&Engine::onResize>(this);
	eventDispatcher->get<Events::WindowClose>()->attach<&Engine::onClose>(this);
	eventDispatcher->get<Events::WindowMinimized>()->attach<&Engine::onMinimized>(this);
	eventDispatcher->get<Events::WindowRestored>()->attach<&Engine::onRestore>(this);
	eventDispatcher->get<Events::WindowRestored>()->attach<&Engine::onRestore>(this);

	Engine::cacheManager = std::make_unique<CacheManager>();
	result = cacheManager->init();
	assert(result == 0);

	Engine::window = std::make_unique<Window>();
	result = window->init();
	assert(result == 0);
	window->setEventHandler(Window::handle_type::create<&Engine::onEvent>());

	Engine::editor = std::make_unique<Editor>();
	eventDispatcher->get<Events::RenderInitialized>()->attach([this](Events::RenderInitialized*) {
		this->getEditor()->init();
		return false;
	});

	Engine::scene = std::make_unique<Scene>();

	Engine::renderer = std::make_unique<Renderer>();
	result = renderer->init();
	assert(result == 0);

	return 0;
}

void Engine::run()
{
	std::shared_ptr<Cache::Resource> resource;
	Engine::getCache()->getResource("resources/models/backpack.obj", resource);

	auto model = utils::static_pointer_cast<Cache::ObjData>(resource->extra);
	auto entity1 = Engine::getScene()->createEntity("BackPack 1");
	{
		auto& transform = entity1.getComponent<Component::Transform>();
		transform.pos = {3, 0, 0};
		auto& component = entity1.addComponent<Component::Renderable>();
		component.mesh = resource;
		component.materials = model->materials;
	}

	auto entity2 = Engine::getScene()->createEntity("BackPack 2");
	{
		auto& component = entity2.addComponent<Component::Renderable>();
		component.mesh = resource;
		component.materials = model->materials;
	}

	Render::FrameBufferParams params;
	params.width = Engine::getWindow()->getWidth();
	params.height = Engine::getWindow()->getHeight();
	params.bufferTypes = Render::FrameBufferType::ColorAttachment0 | Render::FrameBufferType::Depth | Render::FrameBufferType::Stencil;
	params.colorFormat[0] = Render::ColorFormat::RGBA8;
	params.depthFormat = Render::DepthFormat::Depth24;
	params.stencilFormat = Render::StencilFormat::Stencil8;

	Entity cameraA = Engine::getScene()->createEntity("Camera A");
	{
		auto& transformA = cameraA.getComponent<Component::Transform>();
		transformA.pos = {-0.00355635, -0.164256, 2.49057};
		auto& cameraComponentA = cameraA.addComponent<Component::Camera>(&transformA, params);
		cameraA.addComponent<Component::EditorCamera>(&cameraComponentA).initEvents();
		cameraComponentA.setFar(5);
	}
	// Entity cameraB = Engine::getScene()->createEntity("Camera B");
	//{
	//	auto& transformB = cameraB.getComponent<Component::Transform>();
	//	transformB.pos = {-3.03853, 1.01738, 2.71823};
	//	transformB.rotation = {-6.40002, -37.9002, 0};
	//	auto& cameraComponentB = cameraB.addComponent<Component::Camera>(&transformB, params);
	//	cameraB.addComponent<Component::EditorCamera>(&cameraComponentB).initEvents();
	// }
	//
	// Entity cameraC = Engine::getScene()->createEntity("Camera C");
	//{
	//	auto& transformC = cameraC.getComponent<Component::Transform>();
	//	transformC.pos = {1.88206, -0.279197, 2.37471};
	//	transformC.rotation = {-6.39998, -128.8, 0};
	//	auto& cameraComponentC = cameraC.addComponent<Component::Camera>(&transformC, params);
	//	cameraC.addComponent<Component::EditorCamera>(&cameraComponentC).initEvents();
	// }

	while (running)
	{
		if (!minimized)
		{
			editor->begin();

			renderer->preUpdate();
			renderer->update();
			renderer->postUpdate();

			editor->end();
		}

		window->onUpdate();
	}
}
}  // namespace FearEngine
