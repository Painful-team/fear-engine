#ifndef FEARENGINE_EVENT_KEYEVENT_H__
#define FEARENGINE_EVENT_KEYEVENT_H__

#include "Event.hpp"
#include "keyCodes.hpp"

namespace FearEngine::Events
{
class KeyPressed: public Event
{
public:
	KeyPressed(const keys key);

	keys keyCode() const;
private:
	keys key;
};

class KeyReleased: public Event
{
public:
	KeyReleased(const keys key);

	keys keyCode() const;
private:
	keys key;
};

class KeyTyped: public Event
{
public:
	KeyTyped(const keys key);

	keys keyCode() const;
private:
	keys key;
};
}

#endif
