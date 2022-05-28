#include "Event.hpp"

namespace FearEngine::Events
{
bool Event::inCategory(const int category) const { return category & getCategory(); }
}  // namespace FearEngine::Events
