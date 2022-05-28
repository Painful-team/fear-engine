#ifndef FEARENGINE_EVENT_WINDOWEVENT_H__
#define FEARENGINE_EVENT_WINDOWEVENT_H__

#include "Event.hpp"

namespace FearEngine::Events
{
class WindowResize: public Event
{
public:
	WindowResize(const int width, const int heigth);

	uint32_t getWidth() const;
	uint32_t getHeight() const;

	GENCLASSESSETIALS(windowResize, window)
private:
	uint32_t width;
	uint32_t heigth;
};

class WindowClose: public Event
{
public:
	WindowClose();

	GENCLASSESSETIALS(windowClose, window)
};

class WindowFocus: public Event
{
public:
	WindowFocus();

	GENCLASSESSETIALS(windowFocus, window)
};

class WindowLostFocus: public Event
{
public:
	WindowLostFocus();

	GENCLASSESSETIALS(windowLostFocus, window)
};

class WindowMoved: public Event
{
public:
	WindowMoved(const int x, const int y);

	int getX() const;
	int getY() const;

	GENCLASSESSETIALS(windowMoved, window)
private:
	int offsetX;
	int offsetY;
};

class WindowMinimized: public Event
{
public:
	WindowMinimized();

	GENCLASSESSETIALS(windowIconified, window)
};

class WindowRestored: public Event
{
public:
	WindowRestored();

	GENCLASSESSETIALS(windowRestored, window)
};
}  // namespace FearEngine::Events

#endif
