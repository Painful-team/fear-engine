#include "Engine.hpp"

#include <GLFW/glfw3.h>

#include <event/detail/Delegate.hpp>

#include "Window.hpp"
#include "events.hpp"

namespace FearEngine
{
Render::Renderer* Engine::renderer;
Events::Dispatcher* Engine::eventDispatcher;
Window* Engine::window;

Render::Renderer* Engine::getRender()
{
	return renderer;
}

Window* Engine::getWindow()
{
	return window;
}

FearEngine::Events::Dispatcher* Engine::getDispatcher()
{
	return eventDispatcher;
}

void Engine::onEvent(Events::Event* event)
{
	eventDispatcher->notify(event);
}

void Engine::onResize(Events::WindowResize* event)
{
	if (event->getWidth() == 0 && event->getHeigth() == 0)
	{
		minimized = true;
		return;
	}

	minimized = false;
	renderer->onResize(event);
}

void Engine::onClose(Events::WindowClose* event)
{
	running = false;
}

void Engine::provide(Render::Renderer* newrenderer)
{
	renderer = newrenderer;
}

void Engine::provide(Events::Dispatcher* newDispatcher)
{
	eventDispatcher = newDispatcher;
}

void Engine::provide(Window* newWindow)
{
	window = newWindow;
}

int Engine::init()
{
	Engine::provide(new Render::Renderer);
	if (renderer->init() != 0)
	{
		return -1;
	}

	Engine::provide(new Window);

	window->setEventHandler(Window::handle_type::create<&Engine::onEvent>());

	if (window->init(true) != 0)
	{
		return -1;
	}

	Engine::provide(new Events::Dispatcher);

	eventDispatcher->get<Events::WindowResize>(Events::EventType::windowResize)->attach<Engine, &Engine::onResize>(this);
	eventDispatcher->get<Events::WindowClose>(Events::EventType::windowClose)->attach<Engine, &Engine::onClose>(this);

	return 0;
}

void Engine::run()
{
	glfwMakeContextCurrent(window->window);

	while (running)
	{
		glfwSwapBuffers(window->window);
		glfwPollEvents();
	}
}

Engine::~Engine()
{
	delete renderer;
	delete window;
	delete eventDispatcher;
}
}
