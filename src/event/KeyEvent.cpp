#include "KeyEvent.hpp"

#include "Event.hpp"
#include "keyCodes.hpp"

namespace FearEngine::Events
{
KeyPressed::KeyPressed(const keys key)
 : key(key)
{}

keys KeyPressed::keyCode() const { return key; }

KeyReleased::KeyReleased(const keys key)
 : key(key)
{}

keys KeyReleased::keyCode() const { return key; }

KeyTyped::KeyTyped(const keys key)
 : key(key)
{}

keys KeyTyped::keyCode() const { return key; }
}  // namespace FearEngine::Events
