#include <core/Engine.hpp>
#include "Window.hpp"

#include <GLFW/glfw3.h>

#include "Input.hpp"
#include "events.hpp"

#include <core/Logger.hpp>
#include <render/ErrorCodes.hpp>

namespace FearEngine
{
Window::Window(const std::string& title, const uint32_t width, const uint32_t height, bool vsync)
{
	data.title = title;
	data.width = width;
	data.height = height;

	blocked = false;

	setVsync(vsync);
}

uint32_t Window::getWidth() const { return data.width; }

uint32_t Window::getHeight() const { return data.height; }

void Window::setEventHandler(handle_type handler) { data.eventHandler = handler; }

bool Window::isVsync() const { return data.vsync; }

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

bool Window::isCursorBlocked() { return blocked; }

void Window::blockCursor()
{
	blocked = true;
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Window::unblockCursor()
{
	blocked = false;
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void glfwCallbackFunction(int glfwErrorCode, const char* description)
{
	Engine::logs()->error("Core", fmt::format(fmt::fg(fmt::color::red), "Window API error occured with Error code {0} it means \"{1}\".",
									   glfwErrorCode, description));
}

int Window::init(const bool resizeAble)
{
	Engine::logs()->log("Core", "Window initialization has begun.");

	glfwSetErrorCallback(glfwCallbackFunction);

	auto apiResult = glfwInit();
	assert(apiResult);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, resizeAble);

	window = glfwCreateWindow(data.width, data.height, data.title.c_str(), NULL, NULL);
	if (!window)
	{
		Engine::logs()->error("Core", "Window initialization has failed.");

		return Render::errorCodes::WINDOW_INITIALIZATION_FAILED;
	}

	glfwSetWindowUserPointer(window, &data);

	glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height) {
		WindowData* windowData = reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));

		windowData->height = height;
		windowData->width = width;

		auto evnt = Events::WindowResize(width, height);
		windowData->eventHandler(&evnt);
	});

	glfwSetWindowCloseCallback(window, [](GLFWwindow* window) {
		WindowData* windowData = reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));

		auto evnt = Events::WindowClose();
		windowData->eventHandler(&evnt);
	});

	glfwSetWindowPosCallback(window, [](GLFWwindow* window, int x, int y) {
		WindowData* windowData = reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));

		auto evnt = Events::WindowMoved(x, y);
		windowData->eventHandler(&evnt);
	});

	glfwSetWindowFocusCallback(window, [](GLFWwindow* window, int focused) {
		WindowData* windowData = reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));

		if (focused)
		{
			auto evnt = Events::WindowFocus();
			windowData->eventHandler(&evnt);
		}
		else
		{
			auto evnt = Events::WindowLostFocus();
			windowData->eventHandler(&evnt);
		}
	});

	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		WindowData* windowData = reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));

		switch (action)
		{
		case GLFW_PRESS:
		case GLFW_REPEAT: {
			auto evnt = Events::KeyPressed(key);
			windowData->eventHandler(&evnt);
			break;
		}
		case GLFW_RELEASE: {
			auto evnt = Events::KeyReleased(key);
			windowData->eventHandler(&evnt);
		}
		break;
		}
	});

	glfwSetCharCallback(window, [](GLFWwindow* window, uint32_t keycode) {
		WindowData* windowData = reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));

		auto evnt = Events::KeyTyped(keycode);
		windowData->eventHandler(&evnt);
	});

	glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) {
		WindowData* windowData = reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));

		switch (action)
		{
		case GLFW_PRESS: {
			auto evnt = Events::MouseButtonPressed(button);
			windowData->eventHandler(&evnt);
			break;
		}
		case GLFW_RELEASE: {
			auto evnt = Events::MouseButtonReleased(button);
			windowData->eventHandler(&evnt);
			break;
		}
		}
	});

	glfwSetScrollCallback(window, [](GLFWwindow* window, double offsetX, double offsetY) {
		WindowData* windowData = reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));

		auto evnt = Events::MouseScrolled(offsetX, offsetY);
		windowData->eventHandler(&evnt);
	});

	glfwSetCursorPosCallback(window, [](GLFWwindow* window, double x, double y) {
		WindowData* windowData = reinterpret_cast<WindowData*>(glfwGetWindowUserPointer(window));

		auto evnt = Events::MouseMoved(x, y);
		windowData->eventHandler(&evnt);
	});

	glfwMakeContextCurrent(window);

	Engine::logs()->log("Core", "Window initialization has ended successfully.");

	return Render::errorCodes::OK;
}

void Window::onUpdate()
{
	glfwSwapBuffers(window);
	glfwPollEvents();
}
}  // namespace FearEngine
