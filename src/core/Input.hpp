#ifndef FEARENGINE_INPUT_H__
#define FEARENGINE_INPUT_H__

#include <event/MouseCodes.hpp>
#include <event/keyCodes.hpp>
#include <glm/glm.hpp>

namespace FearEngine::Input
{
bool isKeyPressed(const Events::keys key);
bool isKeyReleased(const Events::keys key);

bool isMousePressed(const Events::mouseCode button);
bool isMouseReleased(const Events::mouseCode button);

glm::vec2 getMousePos();
double getMouseX();
double getMouseY();
}  // namespace FearEngine::Input

#endif
