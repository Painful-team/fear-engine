#ifndef FEARENGINE_CORE_ENGINE_H__
#define FEARENGINE_CORE_ENGINE_H__
#include <render/FrameBuffer.hpp>

#include <event/Dispatcher.hpp>
#include <render/Renderer.hpp>
#include <cache/CacheManager.hpp>

#include "Window.hpp"
#include "events.hpp"

#include "Scene.hpp"
#include "Logger.hpp"

namespace FearEngine
{
class Editor;
class Engine
{
public:
	Engine() = default;

	static std::unique_ptr<Renderer>& getRender();
	static std::unique_ptr<Window>& getWindow();
	static std::unique_ptr<Events::Dispatcher>& getDispatcher();
	static std::unique_ptr<CacheManager>& getCache();
	static std::unique_ptr<Editor>& getEditor();
	static std::unique_ptr<Scene>& getScene();
	static std::unique_ptr<Logger>& logs();

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
	static std::unique_ptr<CacheManager> cacheManager;
	static std::unique_ptr<Editor> editor;

	static std::unique_ptr<Scene> scene;
	static std::unique_ptr<Logger> loggers;
};
}  // namespace FearEngine

#endif
