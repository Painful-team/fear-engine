#ifndef FEARENGINE_RENDER_RENDERER_H__
#define FEARENGINE_RENDER_RENDERER_H__

#include <list>
#include <unordered_map>
#include <memory>

#include <event/WindowEvent.hpp>

#include "Layer.hpp"

#include "shader/Shader.hpp"

namespace FearEngine
{
namespace Render
{
using debugProperty = short;
namespace debugProperties
{
enum : short
{
	None = 0,
	Normals = 1 << 0
};
}
};

class Renderer
{
public:
	Renderer() = default;

	int init();
	void preUpdate();
	void update();
	void postUpdate();

	void onResize(Events::WindowResize* event, const int x = 0, const int y = 0);

	~Renderer();

	Render::debugProperty enabledDebugProperties;
private:
	int initGraphicData();

	std::list<Render::Layer*> m_layers;

	std::unordered_map<int, int> graphicsData;

	friend class Render::Shaders::Shader;
};
}  // namespace FearEngine

#endif
