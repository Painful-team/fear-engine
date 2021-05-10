#ifndef FEARENGINE_EVENT_DISPATCHER_H__
#define FEARENGINE_EVENT_DISPATCHER_H__

#include <array>
#include "Event.hpp"

#include "Observer.hpp"

namespace FearEngine::Events
{

class Dispatcher
{
public:
	Dispatcher();

	void notify(Event* event);

	template<class T>
	Observer<void(T*)>* get(EventType type)
	{
		return reinterpret_cast<Observer<void(T*)>*>(events[static_cast<int>(type)]);
	}

	void detach(EventType type, std::pair<uint32_t, uint32_t>& key);

	~Dispatcher();

private:
	std::array<ObserverBase*, static_cast<int>(EventType::invalid)> events;
};
}

#endif
