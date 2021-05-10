#include "MouseEvent.hpp"

#include "Event.hpp"
#include "MouseCodes.hpp"

namespace FearEngine::Events
{
MouseScrolled::MouseScrolled(const double offsetX, const double offsetY):
	offsetX(offsetX),
	offsetY(offsetY),
	Event(EventType::mouseScrolled, EventCategory::mouse | EventCategory::input)
{}

double MouseScrolled::getXoffset() const
{
	return offsetX;
}

double MouseScrolled::getYoffset() const
{
	return offsetY;
}

MouseMoved::MouseMoved(const double x, const double y):
	x(x),
	y(y),
	Event(EventType::mouseMoved, EventCategory::mouse | EventCategory::input)
{}

double MouseMoved::getX() const
{
	return x;
}

double MouseMoved::getY() const
{
	return y;
}

MouseButtonReleased::MouseButtonReleased(const mouseCode button):
	button(button),
	Event(EventType::mouseButtonReleased, EventCategory::mouse | EventCategory::mouseButton | EventCategory::input)
{}

mouseCode MouseButtonReleased::getButton() const
{
	return button;
}

MouseButtonPressed::MouseButtonPressed(const mouseCode button):
	button(button),
	Event(EventType::mouseButtonPressed, EventCategory::mouse | EventCategory::mouseButton | EventCategory::input)
{}

mouseCode MouseButtonPressed::getButton() const
{
	return button;
}
}
