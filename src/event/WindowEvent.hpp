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
	uint32_t getHeigth() const;

private:
	uint32_t width;
	uint32_t heigth;
};

class WindowClose: public Event
{
public:
	WindowClose();
};

class WindowFocus: public Event
{
public:
	WindowFocus();
};

class WindowLostFocus: public Event
{
public:
	WindowLostFocus();
};

class WindowMoved: public Event
{
public:
	WindowMoved(const int x, const int y);

	int getX() const;
	int getY() const;

private:
	int offsetX;
	int offsetY;
};
}

#endif
