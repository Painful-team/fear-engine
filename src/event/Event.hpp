#ifndef FEARENGINE_EVENT_EVENT_H__
#define FEARENGINE_EVENT_EVENT_H__

namespace FearEngine::Events
{
using uint32_t = unsigned;

enum class EventType
{
	windowClose,
	windowResize,
	windowFocus,
	windowLostFocus,
	windowMoved,

	keyPressed,
	keyReleased,
	keyTyped,

	mouseButtonPressed,
	mouseButtonReleased,
	mouseMoved,
	mouseScrolled,

	invalid
};

enum EventCategory
{
	invalid		= 0,
	window		= 1 << 0,
	input		= 1 << 1,
	keyboard	= 1 << 2,
	mouse		= 1 << 3,
	mouseButton 	= 1 << 4
};

class Event
{
public:
	Event(EventType type, const int category);

	EventType getType() const;
	int getCategory() const;

	bool inCategory(const int category);

private:
	EventType type;
	int category;
};
}

#endif
