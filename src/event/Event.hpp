#ifndef FEARENGINE_EVENT_EVENT_H__
#define FEARENGINE_EVENT_EVENT_H__

namespace FearEngine::Events
{
using uint32_t = unsigned;

enum class EventType
{
	renderInit,

	windowClose,
	windowResize,
	windowFocus,
	windowLostFocus,
	windowMoved,
	windowIconified,
	windowRestored,

	guiUpdate,

	keyPressed,
	keyReleased,
	keyTyped,

	mouseButtonPressed,
	mouseButtonReleased,
	mouseMoved,
	mouseScrolled,
	mouseRequired,

	activeViewport,

	// According to static allocation and more flexible way of creating new event Type, "invalid" type should be always on the last position
	invalid
};

enum EventCategory
{
	invalid = 0,
	window = 1 << 0,
	input = 1 << 1,
	keyboard = 1 << 2,
	mouse = 1 << 3,
	mouseButton = 1 << 4,
	core = 1 << 5
};

#ifdef DEBUG
#define GENCLASSESSETIALS(classType, category)                     \
	static EventType staticType() { return EventType::classType; } \
                                                                   \
	EventType type() const { return staticType(); }                \
                                                                   \
	int getCategory() const { return category; }                   \
                                                                   \
	const char* name() const { return #classType; }
#else
#define GENCLASSESSETIALS(classType, category)                     \
	static EventType staticType() { return EventType::classType; } \
                                                                   \
	EventType type() const { return staticType(); }                \
                                                                   \
	int getCategory() const { return category; }
#endif

class Event
{
public:
	virtual EventType type() const = 0;

	virtual int getCategory() const = 0;
#ifdef _DEBUG
	virtual const char* name() const = 0;
#endif

	bool inCategory(const int category) const;
};
}  // namespace FearEngine::Events

#endif
