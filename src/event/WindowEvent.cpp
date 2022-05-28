#include "WindowEvent.hpp"

#include "Event.hpp"

namespace FearEngine::Events
{
WindowResize::WindowResize(const int width, const int heigth)
 : width(width)
 , heigth(heigth)
{}

uint32_t WindowResize::getWidth() const { return width; }

uint32_t WindowResize::getHeight() const { return heigth; }

WindowClose::WindowClose() {}

WindowFocus::WindowFocus() {}

WindowLostFocus::WindowLostFocus() {}

WindowMoved::WindowMoved(const int x, const int y)
 : offsetX(x)
 , offsetY(x)
{}

int WindowMoved::getX() const { return offsetX; }

int WindowMoved::getY() const { return offsetY; }

WindowMinimized::WindowMinimized() {}

WindowRestored::WindowRestored() {}
}  // namespace FearEngine::Events
