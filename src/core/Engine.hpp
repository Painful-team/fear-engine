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
	Events::Dispatcher eventDispatcher;

	Engine() = default;

	static Render::Renderer* getRender();
	static Window* getWindow();

	static void provide(Render::Renderer* renderer);
	static void provide(Window* window);

	int init();
	void run();

	void onEvent(Events::Event* event);
	void onResize(Events::WindowResize* event);
	void onClose(Events::WindowClose* event);

	~Engine();

private:
	bool running = true;
	bool minimized = false;

	static Render::Renderer* renderer;
	static Window* window;
};
}

#endif
