#include "Input.hpp"

#include <GLFW/glfw3.h>

#include "Engine.hpp"

namespace FearEngine::Input
{
bool isKeyPressed(const Events::keys key)
{
	return GLFW_PRESS == glfwGetKey(Engine::getWindow()->window, key);
}

bool isKeyReleased(const Events::keys key)
{
	return GLFW_RELEASE == glfwGetKey(Engine::getWindow()->window, key);
}

bool isMousePressed(const Events::mouseCode button)
{
	return GLFW_PRESS == glfwGetMouseButton(Engine::getWindow()->window, button);
}

bool isMouseReleased(const Events::mouseCode button)
{
	return GLFW_RELEASE == glfwGetMouseButton(Engine::getWindow()->window, button);
}

glm::vec2 getMousePos()
{
	double x;
	double y;

	glfwGetCursorPos(Engine::getWindow()->window, &x, &y);
	return { static_cast<float>(x), static_cast<float>(y) };
}

double getMouseX()
{
	return getMousePos().x;
}

double getMouseY()
{
	return getMousePos().y;
}
}
