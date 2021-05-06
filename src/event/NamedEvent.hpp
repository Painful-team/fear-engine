#ifndef FEARENGINE_SRC_EVENT_NAMEDEVENT_H__
#define FEARENGINE_SRC_EVENT_NAMEDEVENT_H__

#include <unordered_map>
#include <string>
#include <list>

#include "detail/Delegate.hpp"

namespace FearEngine::Events
{
	template<class Lambda>
	class NamedEvent
	{
	public:
		template <typename ...Params>
		void send(const std::string& eventName, Params... arg) const
		{
			(events.at(eventName))(arg...);
		}

		template<class Lambda>
		void receive(const std::string& eventName, const Lambda& function)
		{
			events.insert({ eventName, {function} });
		}

		void receive(const std::string& eventName, const detail::delegate<Lambda>& function)
		{
			events.insert({eventName, function});
		}

		void remove(const std::string& eventName)
		{
			events.erase(eventName);
		}

	private:
		std::unordered_map<std::string, detail::delegate<Lambda>> events;
	};
}

#endif
