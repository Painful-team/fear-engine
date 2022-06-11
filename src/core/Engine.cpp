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

#include "Entity.hpp"

namespace FearEngine
{
std::unique_ptr<Renderer> Engine::renderer;
std::unique_ptr<Events::Dispatcher> Engine::eventDispatcher;
std::unique_ptr<Window> Engine::window;
std::unique_ptr<CacheManager> Engine::cacheManager;
std::unique_ptr<Editor> Engine::editor;
std::unique_ptr<Scene> Engine::scene;

void Engine::onEvent(Events::Event* event) { eventDispatcher->notify(event); }

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

std::unique_ptr<Scene>& Engine::getScene()
{ return Engine::scene; }

int Engine::init()
{
	Engine::renderer = std::make_unique<Renderer>();
	Engine::eventDispatcher = std::make_unique<Events::Dispatcher>();
	Engine::window = std::make_unique<Window>();
	Engine::cacheManager = std::make_unique<CacheManager>();
	Engine::editor = std::make_unique<Editor>();
	Engine::scene = std::make_unique<Scene>();

	eventDispatcher->get<Events::WindowRestored>()->attach<&Engine::onRestore>(this);

	eventDispatcher->get<Events::WindowResize>()->attach<&Engine::onResize>(this);
	eventDispatcher->get<Events::WindowClose>()->attach<&Engine::onClose>(this);
	eventDispatcher->get<Events::WindowMinimized>()->attach<&Engine::onMinimized>(this);
	eventDispatcher->get<Events::WindowRestored>()->attach<&Engine::onRestore>(this);

	assert(cacheManager->init() == 0);

	assert(glfwInit());

	window->setEventHandler(Window::handle_type::create<&Engine::onEvent>());

	assert(window->init(true) == 0);

	glfwMakeContextCurrent(window->window);
	
	eventDispatcher->get<Events::WindowRestored>()->attach<&Engine::onRestore>(this);

	eventDispatcher->get<Events::RenderInitialized>()->attach([this](Events::RenderInitialized*) {
		this->getEditor()->init();
		return false;
	});
	assert(renderer->init() == 0);

	return 0;
}


void Engine::run()
{
	Render::FrameBufferParams params;
	params.width = Engine::getWindow()->getWidth();
	params.height = Engine::getWindow()->getHeight();
	params.bufferTypes = Render::FrameBufferType::Color | Render::FrameBufferType::Depth | Render::FrameBufferType::Stencil;
	params.colorFormat = Render::ColorFormat::RGBA8;
	params.depthFormat = Render::DepthFormat::Depth24;
	params.stencilFormat = Render::StencilFormat::Stencil8;


	Entity cameraA = Engine::getScene()->createEntity("Camera A");
	{
		auto& transformA = cameraA.getComponent<Component::Transform>();
		transformA.pos = {-0.00355635, -0.164256, 2.49057};
		auto& cameraComponentA = cameraA.addComponent<Component::Camera>(&transformA, params);
		cameraA.addComponent<Component::NoclipCameraController>(&cameraComponentA).initEvents();
	}

	Entity cameraB = Engine::getScene()->createEntity("Camera B");
	{
		auto& transformB = cameraB.getComponent<Component::Transform>();
		transformB.pos = {-3.03853, 1.01738, 2.71823};
		transformB.rotation = {-6.40002, -37.9002, 0};
		auto& cameraComponentB = cameraB.addComponent<Component::Camera>(&transformB, params);
		cameraB.addComponent<Component::NoclipCameraController>(&cameraComponentB).initEvents();
	}

	Entity cameraC = Engine::getScene()->createEntity("Camera C");
	{
		auto& transformC = cameraC.getComponent<Component::Transform>();
		transformC.pos = {1.88206, -0.279197, 2.37471};
		transformC.rotation = {-6.39998, -128.8, 0};
		auto& cameraComponentC = cameraC.addComponent<Component::Camera>(&transformC, params);
		cameraC.addComponent<Component::NoclipCameraController>(&cameraComponentC).initEvents();
	}

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
