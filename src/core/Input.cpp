#include "Input.hpp"

#include <GLFW/glfw3.h>

#include "Engine.hpp"

namespace FearEngine
{
bool Input::isKeyPressed(const Events::keys key)
{
	return GLFW_PRESS == glfwGetKey(Engine::getWindow()->window, key);
}

bool Input::isKeyReleased(const Events::keys key)
{
	return GLFW_RELEASE == glfwGetKey(Engine::getWindow()->window, key);
}

bool Input::isMousePressed(const Events::mouseCode button)
{
	return GLFW_PRESS == glfwGetMouseButton(Engine::getWindow()->window, button);
}

bool Input::isMouseReleased(const Events::mouseCode button)
{
	return GLFW_RELEASE == glfwGetMouseButton(Engine::getWindow()->window, button);
}

glm::vec2 Input::getMousePos()
{
	double x;
	double y;

	glfwGetCursorPos(Engine::getWindow()->window, &x, &y);
	return { static_cast<float>(x), static_cast<float>(y) };
}

double Input::getMouseX()
{
	return getMousePos().x;
}

double Input::getMouseY()
{
	return getMousePos().y;
}
}
