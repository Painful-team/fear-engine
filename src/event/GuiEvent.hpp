#ifndef FEARENGINE_EVENT_GUIEVENT_H__
#define FEARENGINE_EVENT_GUIEVENT_H__

#include "Event.hpp"

namespace FearEngine::Component
{
class Camera;
}

namespace FearEngine::Events
{
class GuiUpdate: public Event
{
public:
	GuiUpdate();

	GENCLASSESSETIALS(guiUpdate, window)
};

class ActiveViewport: public Event
{
public:
	ActiveViewport(Component::Camera* activeEnt);

	FearEngine::Component::Camera*& getActive();

	GENCLASSESSETIALS(activeViewport, input)

private:
	Component::Camera* activeCamera;
};
}  // namespace FearEngine::Events
#endif
