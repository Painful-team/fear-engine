#include "GuiEvent.hpp"

namespace FearEngine::Events
{
FearEngine::Events::GuiUpdate::GuiUpdate():
	Event(EventType::guiUpdate, EventCategory::window)
{}
}
