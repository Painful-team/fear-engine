#include "Dispatcher.hpp"
#include "WindowEvent.hpp"
#include "KeyEvent.hpp"
#include "MouseEvent.hpp"
#include "GuiEvent.hpp"
#include "CoreEvent.hpp"
#include "Event.hpp"
#include "Observer.hpp"

namespace FearEngine::Events
{
#define NOTCASE(casetype, input, Class, func)										\
casetype(input == Class::staticType())												\
{																					\
	auto* ptr = reinterpret_cast<Observer<bool(Class*)>*>(events[getIndex(input)]);	\
	auto* newType = reinterpret_cast<Class*>(func);									\
	for (auto it = ptr->begin(); it != ptr->end(); ++it)							\
	{																				\
		if (!(*it)(newType))														\
		{																			\
			break;																	\
		}																			\
	}																				\
}

#define FUNCASE(casetype, input, Class, func)									\
casetype(input == Class::staticType())											\
{																				\
	reinterpret_cast<Observer<bool(Class*)>*>(events[getIndex(input)])->func;	\
}                                                              					\

#define GETOBSERVER(type, cases, func)			 		 \
	cases(if,		type,	WindowClose,		 	func)\
	cases(else if,	type,	WindowResize, 			func)\
	cases(else if, 	type,	WindowFocus, 			func)\
	cases(else if, 	type,	WindowLostFocus,		func)\
	cases(else if, 	type,	WindowMoved, 			func)\
	cases(else if, 	type,	WindowMinimized, 			func)\
	cases(else if, 	type,	WindowRestored, 			func)\
														 \
	cases(else if, 	type,	GuiUpdate, 				func)\
														 \
	cases(else if, 	type,	KeyPressed, 			func)\
	cases(else if, 	type,	KeyReleased, 			func)\
	cases(else if, 	type,	KeyTyped, 				func)\
														 \
	cases(else if, 	type,	MouseButtonPressed, 	func)\
	cases(else if, 	type,	MouseButtonReleased, 	func)\
	cases(else if, 	type,	MouseMoved, 			func)\
	cases(else if, 	type,	MouseScrolled, 			func)\
	cases(else if, type, RenderInitialized, func)\
	cases(else if, type, MouseRequired, func)\
	cases(else if, type, ActiveViewport, func)\

Dispatcher::Dispatcher()
{
	registerEvent<WindowClose>();
	registerEvent<WindowResize>();
	registerEvent<WindowFocus>();
	registerEvent<WindowLostFocus>();
	registerEvent<WindowMoved>();
	registerEvent<WindowMinimized>();
	registerEvent<WindowRestored>();

	registerEvent<GuiUpdate>();

	registerEvent<KeyPressed>();
	registerEvent<KeyReleased>();
	registerEvent<KeyTyped>();

	registerEvent<MouseButtonPressed>();
	registerEvent<MouseRequired>();
	registerEvent<MouseButtonReleased>();
	registerEvent<MouseMoved>();
	registerEvent<MouseScrolled>();
	registerEvent<RenderInitialized>();
	registerEvent<ActiveViewport>();
}

void Dispatcher::notify(Event* event)
{
	GETOBSERVER(event->type(), NOTCASE, event);
}

Dispatcher::~Dispatcher()
{
	for (auto it : events)
	{
		delete it;
	}
}

constexpr int Dispatcher::getIndex(EventType type)
{
	return static_cast<int>(type);
}

void Dispatcher::detach(EventType type, std::pair<uint32_t, uint32_t>& key)
{
	GETOBSERVER(type, FUNCASE, detach(key));
}
}
