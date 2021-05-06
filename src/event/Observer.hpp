#ifndef FEARENGINE_SRC_EVENT_OBSERVER_H__
#define FEARENGINE_SRC_EVENT_LISTENER_H__

#include <vector>

#include "detail/Delegate.hpp"

namespace FearEngine::Events
{

template<class Lambda>
class Observer
{
public:
	template <typename... Params>
	void notify(Params... arg) const
	{
		for (size_t i = 0; i < subscribers.size(); ++i)
		{
			subscribers[i](arg...);
		}
	}

	template<class Lambda>
	size_t attach(const Lambda& function)
	{
		subscribers.push_back({ function });
		return subscribers.size() - 1;
	}

	size_t attach(const detail::delegate<Lambda>& function)
	{
		subscribers.push_back(function);
		return subscribers.size() - 1;
	}

	void detach(uint32_t id)
	{
		subscribers.erase(vec.begin() + id);
	}

private:
	std::vector<detail::delegate<Lambda>> subscribers;
};
}

#endif
