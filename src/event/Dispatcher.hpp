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

	template <class T>
	auto get()
	{
		return reinterpret_cast<Observer<bool(T*)>*>(events[static_cast<int>(T::staticType())]);
	}

	void detach(EventType type, std::pair<uint32_t, uint32_t>& key);

	~Dispatcher();

private:
	constexpr int getIndex(EventType type);

	template <typename T>
	constexpr void registerEvent()
	{
		events[getIndex(T::staticType())] = new Observer<bool(T*)>;
	}

	std::array<ObserverBase*, static_cast<int>(EventType::invalid)> events;
};
}  // namespace FearEngine::Events

#endif
