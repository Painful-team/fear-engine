#ifndef FEARENGINE_INPUT_H__
#define FEARENGINE_INPUT_H__

#include <glm/glm.hpp>
#include <event/keyCodes.hpp>
#include <event/MouseCodes.hpp>

namespace FearEngine
{
namespace Input
{
bool isKeyPressed(const Events::keys key);
bool isKeyReleased(const Events::keys key);

bool isMousePressed(const Events::mouseCode button);
bool isMouseReleased(const Events::mouseCode button);

glm::vec2 getMousePos();
double getMouseX();
double getMouseY();
};
}

#endif
