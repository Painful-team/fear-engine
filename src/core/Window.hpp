#ifndef FEARENGINE_CORE_WINDOW_H__
#define FEARENGINE_CORE_WINDOW_H__

#include <GLFW/glfw3.h>
#include <string>

#include <event/Event.hpp>
#include <event/detail/Delegate.hpp>

#include <core/Input.hpp>

namespace FearEngine
{
class Editor;

class Window
{
public:
	static constexpr const uint32_t defaultWidth = 1920;
	static constexpr const uint32_t defaultHeight = 1080;

	using event_sign = void(Events::Event*);
	using handle_type = Events::detail::Delegate<event_sign>;

	struct WindowData
	{
		std::string title;
		uint32_t width;
		uint32_t height;
		bool vsync;

		handle_type eventHandler;
	};

	Window(const std::string& title = "FearEngine",
		 const uint32_t width = defaultWidth,
		 const uint32_t heigth = defaultHeight,
		 bool vsync = false);

	uint32_t getWidth() const;
	uint32_t getHeight() const;

	void setEventHandler(handle_type handler);

	bool isVsync() const;
	void setVsync(const bool vsync);

	bool isCursorBlocked();
	void blockCursor();
	void unblockCursor();

	int init(const bool resizeAble = true);

	void onUpdate();

private:
	WindowData data;
	GLFWwindow* window;

	int blocked;

	friend bool Input::isKeyPressed(const Events::keys key);
	friend bool Input::isKeyReleased(const Events::keys key);

	friend bool Input::isMousePressed(const Events::mouseCode button);
	friend bool Input::isMouseReleased(const Events::mouseCode button);

	friend glm::vec2 Input::getMousePos();

	friend class Editor;
};
}  // namespace FearEngine

#endif
