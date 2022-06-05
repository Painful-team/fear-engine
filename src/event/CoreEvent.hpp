#ifndef FEARENGINE_EVENT_COREEVENT_H__
#define FEARENGINE_EVENT_COREEVENT_H__

#include "Event.hpp"

namespace FearEngine::Events
{
class RenderInitialized: public Event
{
public:
	RenderInitialized();

	GENCLASSESSETIALS(renderInit, core)
};
}
#endif
