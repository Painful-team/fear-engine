#include "Window.hpp"

#include <GLFW/glfw3.h>

#include "events.hpp"

namespace FearEngine
{
Window::Window(const std::string& title, const uint32_t width, const uint32_t heigth, bool vsync)
{
	data.title = title;
	data.width = width;
	data.heigth = heigth;

	setVsync(vsync);
}

uint32_t Window::getWidth() const
{
	return data.width;
}

uint32_t Window::getHeigth() const
{
	return data.heigth;
}

void Window::setEventHandler(handle_type& handler)
{
	data.eventHandler = handler;
}

bool Window::isVsync() const
{
	return data.vsync;
}

void Window::setVsync(const bool vsync)
{
	if (vsync)
	{
		glfwSwapInterval(1);
	}
	else
	{
		glfwSwapInterval(0);
	}

	data.vsync = true;
}

int Window::init(const bool resizeAble)
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, resizeAble);

	window = glfwCreateWindow(data.width, data.heigth, data.title.c_str(), NULL, NULL);
	if (!window)
	{
		return -1;
	}

	glfwSetWindowUserPointer(window, &data);

	glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int heigth)
	{
			WindowData* data = reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));

			data->heigth = heigth;
			data->width = width;

			data->eventHandler(&Events::WindowResize(heigth, heigth));
	});

	glfwSetWindowCloseCallback(window, [](GLFWwindow* window)
	{
			WindowData* data = reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));

			data->eventHandler(&Events::WindowClose());
	});

	glfwSetWindowPosCallback(window, [](GLFWwindow* window, int x, int y)
	{
			WindowData* data = reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));

			data->eventHandler(&Events::WindowMoved(x, y));
	});

	glfwSetWindowFocusCallback(window, [](GLFWwindow* window, int focused)
	{
			WindowData* data = reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));

			if (focused)
			{
				data->eventHandler(&Events::WindowFocus());
			}
			else
			{
				data->eventHandler(&Events::WindowLostFocus());
			}
	});

	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
	{
			WindowData* data = reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));

			switch (action)
			{
			case GLFW_PRESS:
			{
				data->eventHandler(&Events::KeyPressed(key));
				break;
			}
			case GLFW_RELEASE:
			{
				data->eventHandler(&Events::KeyReleased(key));
				break;
			}
			case GLFW_REPEAT:
			{
				data->eventHandler(&Events::KeyPressed(key));
				break;
			}
			}
	});

	glfwSetCharCallback(window, [](GLFWwindow* window, uint32_t keycode)
	{
			WindowData* data = reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));

			data->eventHandler(&Events::KeyTyped(keycode));
	});

	glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods)
	{
			WindowData* data = reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));

			switch (action)
			{
			case GLFW_PRESS:
			{
				data->eventHandler(&Events::MouseButtonPressed(button));
				break;
			}
			case GLFW_RELEASE:
			{
				data->eventHandler(&Events::MouseButtonReleased(button));
				break;
			}
			}
	});

	glfwSetScrollCallback(window, [](GLFWwindow* window, double offsetX, double offsetY)
	{
			WindowData* data = reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));

			data->eventHandler(&Events::MouseScrolled(offsetX, offsetY));
	});

	glfwSetCursorPosCallback(window, [](GLFWwindow* window, double x, double y)
	{
			WindowData* data = reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));

			data->eventHandler(&Events::MouseMoved(x, y));
	});

	return 0;
}
}
