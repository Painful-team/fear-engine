#ifndef FEARENGINE_EVENT_OBSERVER_H__
#define FEARENGINE_EVENT_OBSERVER_H__

#include <slot_map.h>

#include "detail/Delegate.hpp"

namespace FearEngine::Events
{
class ObserverBase
{
public:
	ObserverBase() {};
};

template<typename T>
class Observer;

template<typename Return, typename ...Params>
class Observer<Return(Params...)>: public ObserverBase
{
public:
	Observer() {};

	using func_handle = detail::Delegate<Return(Params...)>;

	void notify(Params... arg) const
	{
		for(auto it: subscribers)
		{
			(it)(arg...);
		}
	}

	template<class Lambda>
	auto attach(const Lambda& function)
	{
		return subscribers.insert({ function });
	}

	template <class T, Return(T::*method)(Params...)>
	auto attach(T* instance)
	{
		return subscribers.insert(func_handle::create<T, method>(instance));
	}

	template <class T, Return(T::*method)(Params...) const>
	auto attach(const T* instance)
	{
		return subscribers.insert(func_handle::create<T, method>(instance));
	}

	auto attach(const func_handle& function)
	{
		return subscribers.insert(function);
	}

	template <Return(*function)(Params...)>
	auto attach()
	{
		return subscribers.insert(func_handle::create<function>());
	}

	void detach(std::pair<uint32_t, uint32_t>& key)
	{
		subscribers.erase(key);
	}

private:
	stdext::slot_map<func_handle> subscribers;
};
}

#endif
