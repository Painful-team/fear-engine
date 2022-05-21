#include "glad/glad.h"
#include "Engine.hpp"

#include <iostream>
#include <memory>
#include <cassert>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

#include <render/shader/Uniform.hpp>
#include <event/detail/Delegate.hpp>
#include <event/KeyCodes.hpp>

#include "Window.hpp"
#include "events.hpp"
#include "Input.hpp"


namespace FearEngine
{
std::unique_ptr<Renderer> Engine::renderer =std::make_unique<Renderer>();
std::unique_ptr <Events::Dispatcher> Engine::eventDispatcher = std::make_unique<Events::Dispatcher>();
std::unique_ptr<Window> Engine::window = std::make_unique<Window>();

void Engine::onEvent(Events::Event* event)
{
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
	renderer->onResize(event);

	return true;
}

bool Engine::onClose(Events::WindowClose* event)
{
	running = false;
	return true;
}

std::unique_ptr<Renderer>& Engine::getRender()
{
	return Engine::renderer;
}

std::unique_ptr<Window>& Engine::getWindow()
{
	return Engine::window;
}

std::unique_ptr<Events::Dispatcher>& Engine::getDispatcher()
{
	return Engine::eventDispatcher;
}

int Engine::init()
{
	assert(glfwInit());

	window->setEventHandler(Window::handle_type::create<&Engine::onEvent>());

	assert(window->init(true) == 0);

	glfwMakeContextCurrent(window->window);

	assert(renderer->init() == 0);

	eventDispatcher->get<Events::WindowRestored>()->attach<&Engine::onRestore>(this);
	
	eventDispatcher->get<Events::WindowResize>()->attach<&Engine::onResize>(this);
	eventDispatcher->get<Events::WindowClose>()->attach<&Engine::onClose>(this);
	eventDispatcher->get<Events::WindowMinimized>()->attach<&Engine::onMinimized>(this);
	eventDispatcher->get<Events::WindowRestored>()->attach<&Engine::onRestore>(this);
	eventDispatcher->get<Events::KeyPressed>()->attach([this](Events::KeyPressed* evnt) 
		{ 
			std::cout << evnt->keyCode();
			if (evnt->keyCode() == Events::Key::ESCAPE)
			{
				std::cout << " ESCAPE" << std::endl;
				auto evnt = Events::WindowClose();
				Engine::getDispatcher()->notify(&evnt);
			}
			std::cout << std::endl;
	
			return false;; 
		}
	);
	return 0;
}

void Engine::run()
{
	while (running)
	{
		renderer->preUpdate();

		renderer->update();
		window->onUpdate();
	}
}
}
