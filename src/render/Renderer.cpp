#include "Renderer.hpp"

#include <GLFW/glfw3.h>

#include <event/WindowEvent.hpp>

namespace FearEngine::Render
{
int Renderer::init()
{
	if (!glfwInit())
	{
		return -1;
	}

	return 0;
}

void Renderer::onResize(Events::WindowResize* event)
{
	glViewport(0, 0, event->getWidth(), event->getHeigth());
}
}
