#include "KeyEvent.hpp"

#include "Event.hpp"
#include "keyCodes.hpp"

namespace FearEngine::Events
{
KeyPressed::KeyPressed(const keys key):
	key(key),
	Event(EventType::keyPressed, EventCategory::keyboard | EventCategory::input)
{}

keys KeyPressed::keyCode() const
{
	return key;
}

KeyTyped::KeyTyped(const keys key):
	key(key),
	Event(EventType::keyTyped, EventCategory::keyboard | EventCategory::input)
{}

keys KeyTyped::keyCode() const
{
	return key;
}

KeyReleased::KeyReleased(const keys key):
	key(key),
	Event(EventType::keyReleased, EventCategory::keyboard | EventCategory::input)
{}

keys KeyReleased::keyCode() const
{
	return key;
}
}
