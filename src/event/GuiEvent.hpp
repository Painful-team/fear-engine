#ifndef FEARENGINE_EVENT_GUIEVENT_H__
#define FEARENGINE_EVENT_GUIEVENT_H__

#include "Event.hpp"

namespace FearEngine::Events
{
class GuiUpdate: public Event
{
public:
	GuiUpdate();

	GENCLASSESSETIALS(guiUpdate, window)
};
}
#endif
