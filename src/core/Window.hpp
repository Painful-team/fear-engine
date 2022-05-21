#ifndef FEARENGINE_CORE_WINDOW_H__
#define FEARENGINE_CORE_WINDOW_H__

#include <string>
#include <GLFW/glfw3.h>

#include <event/Event.hpp>
#include <event/detail/Delegate.hpp>

namespace FearEngine
{
class Window
{
public:
	using event_sign  = void(Events::Event*);
	using handle_type = Events::detail::Delegate<event_sign>;

	struct WindowData
	{
		std::string title;
		uint32_t width;
		uint32_t height;
		bool vsync;

		handle_type eventHandler;
	};

	Window(const std::string& title = "FearEngine", const uint32_t width = 1920, const uint32_t heigth = 1080, bool vsync = false);

	uint32_t getWidth() const;
	uint32_t getHeight() const;

	void setEventHandler(handle_type handler);

	bool isVsync() const;
	void setVsync(const bool vsync);

	int init(const bool resizeAble);

	void onUpdate();

	GLFWwindow* window;
private:
	WindowData data;
};
}

#endif
