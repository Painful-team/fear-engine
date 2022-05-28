#ifndef FEARENGINE_EVENT_OBSERVER_H__
#define FEARENGINE_EVENT_OBSERVER_H__

#include <slot_map.h>
#include "detail/Delegate.hpp"


namespace FearEngine::Events
{
class ObserverBase
{
public:
	ObserverBase(){};
};

template <typename T>
class Observer;

template <typename Return, typename... Params>
class Observer<Return(Params...)>: public ObserverBase
{
public:
	Observer(){};

	using func_handle = detail::Delegate<Return(Params...)>;

	template <typename... Args, typename = std::enable_if_t<std::is_invocable_v<Return (*)(Params...), Args...>>>
	void notify(Args... arg) const
	{
		for (const auto& it : subscribers)
		{
			(it)(std::forward<Args>(arg)...);
		}
	}

	template <class Lambda, typename = std::enable_if_t<detail::detail::has_call_operator<Lambda>::type>>
	auto attach(const Lambda& function)
	{
		return subscribers.emplace(function);
	}

	template <auto method, typename T>
	auto attach(T* instance)
	{
		return subscribers.emplace(func_handle::create<method>(instance));
	}

	template <auto method, typename T>
	auto attach(const T* instance)
	{
		return subscribers.emplace(func_handle::create<method>(instance));
	}

	auto attach(const func_handle& function) { return subscribers.emplace(function); }

	template <Return (*Function)(Params...)>
	auto attach()
	{
		return subscribers.insert(func_handle::template create<Function>(nullptr));
	}

	void detach(std::pair<uint32_t, uint32_t>& key) { subscribers.erase(key); }

	void clear() { subscribers.clear(); }

	auto begin() { return subscribers.begin(); }

	auto end() { return subscribers.end(); }

private:
	stdext::slot_map<func_handle> subscribers;
};
}  // namespace FearEngine::Events

#endif
