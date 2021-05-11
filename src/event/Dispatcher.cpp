#include "Dispatcher.hpp"

#include "WindowEvent.hpp"
#include "KeyEvent.hpp"
#include "MouseEvent.hpp"
#include "GuiUpdate.hpp"
#include "Event.hpp"
#include "Observer.hpp"

namespace FearEngine::Events
{
#define OBS(type) Observer<void(type*)>

#define INDEX(type) static_cast<int>(EventType::type)

#define NOTCASE(type, Class, func) case EventType::type:						\
	reinterpret_cast<OBS(Class)*>(events[INDEX(type)])->notify(reinterpret_cast<Class*>(event));	\
	func												\
	break;

#define FUNCASE(type, Class, func) case EventType::type:		\
	reinterpret_cast<OBS(Class)*>(events[INDEX(type)])->func;	\
	break;

#define GETOBSERVER(type, cases, func)						\
switch (type)									\
{										\
	cases(windowClose,				WindowClose, func)	\
	cases(windowResize,				WindowResize, func)	\
	cases(windowFocus,				WindowFocus, func)	\
	cases(windowLostFocus,				WindowLostFocus, func)	\
	cases(windowMoved,				WindowMoved, func)	\
										\
	cases(guiUpdate,				GuiUpdate, func)	\
										\
	cases(keyPressed,				KeyPressed, func)	\
	cases(keyReleased,				KeyReleased, func)	\
	cases(keyTyped,					KeyTyped, func)		\
										\
	cases(mouseButtonPressed,			KeyPressed, func)	\
	cases(mouseButtonReleased,			KeyReleased, func)	\
	cases(mouseMoved,				KeyTyped, func)		\
	cases(mouseScrolled,				KeyTyped, func)		\
}

Dispatcher::Dispatcher()
{
	events[INDEX(windowClose)]					= new OBS(WindowClose);
	events[INDEX(windowResize)]					= new OBS(WindowResize);
	events[INDEX(windowFocus)]					= new OBS(WindowFocus);
	events[INDEX(windowLostFocus)]					= new OBS(WindowLostFocus);
	events[INDEX(windowMoved)]					= new OBS(WindowMoved);

	events[INDEX(guiUpdate)] = new OBS(GuiUpdate);

	events[INDEX(keyPressed)]					= new OBS(KeyPressed);
	events[INDEX(keyReleased)]					= new OBS(KeyReleased);
	events[INDEX(keyTyped)]						= new OBS(KeyTyped);

	events[INDEX(mouseButtonPressed)]				= new OBS(KeyPressed);
	events[INDEX(mouseButtonReleased)]				= new OBS(KeyReleased);
	events[INDEX(mouseMoved)]					= new OBS(KeyTyped);
	events[INDEX(mouseScrolled)]					= new OBS(KeyTyped);
}

void Dispatcher::notify(Event* event)
{
	GETOBSERVER(event->getType(), NOTCASE, ;);
}

Dispatcher::~Dispatcher()
{
	for (auto it : events)
	{
		delete it;
	}
}

void Dispatcher::detach(EventType type, std::pair<uint32_t, uint32_t>& key)
{
	GETOBSERVER(type, FUNCASE, detach(key));
}
}
