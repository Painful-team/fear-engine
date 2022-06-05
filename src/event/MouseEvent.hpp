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

	GENCLASSESSETIALS(mouseButtonPressed, mouse | input)
private:
	mouseCode button;
};

class MouseRequired: public Event
{
public:
	MouseRequired(const bool required);

	bool isRequired() const;

	GENCLASSESSETIALS(mouseRequired, mouse | input)
private:
	bool state;
};

class MouseButtonReleased: public Event
{
public:
	MouseButtonReleased(const mouseCode button);

	mouseCode getButton() const;

	GENCLASSESSETIALS(mouseButtonReleased, mouse | input)
private:
	mouseCode button;
};

class MouseMoved: public Event
{
public:
	MouseMoved(const double x, const double y);

	double getX() const;
	double getY() const;

	GENCLASSESSETIALS(mouseMoved, mouse | input)
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

	GENCLASSESSETIALS(mouseScrolled, mouse | input)
private:
	double offsetX;
	double offsetY;
};
}  // namespace FearEngine::Events

#endif
