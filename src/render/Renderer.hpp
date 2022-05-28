#ifndef FEARENGINE_RENDER_RENDERER_H__
#define FEARENGINE_RENDER_RENDERER_H__

#include <list>

#include <event/WindowEvent.hpp>

#include "Layer.hpp"

namespace FearEngine
{
class Renderer
{
public:
	Renderer() = default;

	int init();
	void preUpdate();
	void update();
	void onResize(Events::WindowResize* event, const int x = 0, const int y = 0);

	~Renderer();

private:
	std::list<Render::Layer*> m_layers;
};
}  // namespace FearEngine

#endif
