#ifndef FEARENGINE_EVENT_NAMEDEVENT_H__
#define FEARENGINE_EVENT_NAMEDEVENT_H__

#include <unordered_map>
#include <string>

#include "detail/Delegate.hpp"

namespace FearEngine::Events
{
	template <typename T>
	class NamedEvent;

	template<typename Return, typename ...Params>
	class NamedEvent<Return(Params...)>
	{
	public:
		using func_handle = detail::Delegate<Return(Params...)>;

		template <typename ...Params>
		void send(const std::string& eventName, Params... arg) const
		{
			(events.at(eventName))(arg...);
		}

		template<class Lambda>
		auto receive(const std::string& eventName, const Lambda& function)
		{
			return events.insert({ eventName, { function } });
		}

		template <class T, Return(T::* method)(Params...)>
		auto receive(const std::string& eventName, T* instance)
		{
			return events.insert({ func_handle::create<T, method>(instance) });
		}

		template <class T, Return(T::* method)(Params...) const>
		auto receive(const std::string& eventName, const T* instance)
		{
			return events.insert({ func_handle::create<T, method>(instance) });
		}

		auto receive(const std::string& eventName, const func_handle& function)
		{
			return events.insert({ eventName, function });
		}

		template <Return(*function)(Params...)>
		auto receive(const std::string& eventName)
		{
			return events.insert({ eventName, func_handle::create<function>() });
		}

		void remove(const std::string& eventName)
		{
			events.erase(eventName);
		}

	private:
		std::unordered_map<std::string, func_handle> events;
	};
}

#endif
