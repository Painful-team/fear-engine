#ifndef FEARENGINE_RENDER_RENDERER_H__
#define FEARENGINE_RENDER_RENDERER_H__

#include <event/WindowEvent.hpp>

namespace FearEngine::Render
{
class Renderer
{
public:
	Renderer() {};
	int init();
	void onResize(Events::WindowResize* event);

private:
};
}

#endif
