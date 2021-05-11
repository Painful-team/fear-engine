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

	static Render::Renderer* getRender();
	static Window* getWindow();
	static Events::Dispatcher* getDispatcher();

	static void provide(Events::Dispatcher* newDispatcher);
	static void provide(Render::Renderer* newRenderer);
	static void provide(Window* newWindow);

	int init();
	void run();

	static void onEvent(Events::Event* event);
	void onResize(Events::WindowResize* event);
	void onClose(Events::WindowClose* event);

	~Engine();

private:
	bool running = true;
	bool minimized = false;

	static Events::Dispatcher* eventDispatcher;
	static Render::Renderer* renderer;
	static Window* window;
};
}

#endif
