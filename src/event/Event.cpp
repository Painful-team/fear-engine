#include "Event.hpp"

namespace FearEngine::Events
{
Event::Event(EventType type, const int category):
	type(type),
	category(category)
{}

EventType Event::getType() const
{
	return type;
}

int Event::getCategory() const
{
	return category;
}

bool Event::inCategory(const int category)
{
	return category & getCategory();
}

}
