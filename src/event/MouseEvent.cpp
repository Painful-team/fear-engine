#include "MouseEvent.hpp"

#include "Event.hpp"
#include "MouseCodes.hpp"

namespace FearEngine::Events
{
MouseButtonPressed::MouseButtonPressed(const mouseCode button): button(button) {}

mouseCode MouseButtonPressed::getButton() const { return button; }

MouseButtonReleased::MouseButtonReleased(const mouseCode button): button(button) {}

mouseCode MouseButtonReleased::getButton() const { return button; }

MouseMoved::MouseMoved(const double x, const double y): x(x), y(y) {}

double MouseMoved::getX() const { return x; }

double MouseMoved::getY() const { return y; }

MouseScrolled::MouseScrolled(const double offsetX, const double offsetY): offsetX(offsetX), offsetY(offsetY) {}

double MouseScrolled::getXoffset() const { return offsetX; }

double MouseScrolled::getYoffset() const { return offsetY; }
}  // namespace FearEngine::Events
