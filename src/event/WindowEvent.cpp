#include "WindowEvent.hpp"

#include "Event.hpp"

namespace FearEngine::Events
{
WindowResize::WindowResize(const int width, const int heigth):
	width(width),
	heigth(heigth),
	Event(EventType::windowResize, EventCategory::window)
{}

uint32_t WindowResize::getWidth() const
{
	return width;
}

uint32_t WindowResize::getHeigth() const
{
	return heigth;
}

WindowClose::WindowClose():
	Event(EventType::windowClose, EventCategory::window)
{}

WindowFocus::WindowFocus():
	Event(EventType::windowFocus, EventCategory::window)
{}

WindowLostFocus::WindowLostFocus():
	Event(EventType::windowLostFocus, EventCategory::window)
{}

WindowMoved::WindowMoved(const int x, const int y):
	offsetX(x),
	offsetY(x),
	Event(EventType::windowMoved, EventCategory::window)
{}

int WindowMoved::getX() const
{
	return offsetX;
}

int WindowMoved::getY() const
{
	return offsetY;
}
}
