#ifndef FEARENGINE_CORE_ENGINE_H__
#define FEARENGINE_CORE_ENGINE_H__

#include <event/Dispatcher.hpp>
#include <render/Renderer.hpp>

#include "events.hpp"
#include "Window.hpp"

namespace FearEngine
{
class Engine
{
public:

	Engine() = default;

	static std::unique_ptr<Renderer>& getRender();
	static std::unique_ptr<Window>& getWindow();
	static std::unique_ptr<Events::Dispatcher>& getDispatcher();

	int init();
	void run();

	static void onEvent(Events::Event* event);
	bool onResize(Events::WindowResize* event);
	bool onClose(Events::WindowClose* event);
	bool onMinimized(Events::WindowMinimized* event);
	bool onRestore(Events::WindowRestored* event);

private:
	bool running = true;
	bool minimized = false;

	static std::unique_ptr<Events::Dispatcher> eventDispatcher;
	static std::unique_ptr<Renderer> renderer;
	static std::unique_ptr<Window> window;
};
}

#endif
