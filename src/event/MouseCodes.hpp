#ifndef FEARENGINE_EVENT_MOUSECODES_H__
#define FEARENGINE_EVENT_MOUSECODES_H__

namespace FearEngine::Events
{
using mouseCode = short;

namespace Mouse
{
enum: mouseCode
{
	BUTTON_1			= 0,
	BUTTON_2			= 1,
	BUTTON_3			= 2,
	BUTTON_4			= 3,
	BUTTON_5			= 4,
	BUTTON_6			= 5,
	BUTTON_7			= 6,
	BUTTON_8			= 7,
	BUTTON_LAST		= BUTTON_8,
	BUTTON_LEFT		= BUTTON_1,
	BUTTON_RIGHT	= BUTTON_2,
	BUTTON_MIDDLE = BUTTON_3
};
}
}

#endif
