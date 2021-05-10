#ifndef FEARENGINE_EVENT_MOUSEEVENT_H__
#define FEARENGINE_EVENT_MOUSEEVENT_H__

#include "Event.hpp"
#include "MouseCodes.hpp"

namespace FearEngine::Events
{
class MouseButtonPressed: public Event
{
public:
	MouseButtonPressed(const mouseCode button);

	mouseCode getButton() const;

private:
	mouseCode button;
};

class MouseButtonReleased: public Event
{
public:
	MouseButtonReleased(const mouseCode button);

	mouseCode getButton() const;

private:
	mouseCode button;
};

class MouseMoved: public Event
{
public:
	MouseMoved(const double x, const double y);

	double getX() const;
	double getY() const;

private:
	double x;
	double y;
};

class MouseScrolled: public Event
{
public:
	MouseScrolled(const double offsetX, const double offsetY);

	double getXoffset() const;
	double getYoffset() const;

private:
	double offsetX;
	double offsetY;
};
}

#endif
