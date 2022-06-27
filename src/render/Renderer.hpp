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
};

struct RenderStats
{
	uint32_t polygons;
	uint32_t drawCalls;
};

class ModelLayer;
};

class Renderer
{
public:
	Renderer() = default;

	int init();
	void preUpdate();
	void update();
	void postUpdate();

	const Render::RenderStats& getStats() const;

	void onResize(Events::WindowResize* event, const int x = 0, const int y = 0);

	~Renderer();

	Render::debugProperty enabledDebugProperties;
private:
	int initGraphicData();

	Render::RenderStats stats;

	std::list<Render::Layer*> m_layers;

	std::unordered_map<int, int> graphicsData;

	friend class Render::Shaders::Shader;
	friend class Render::ModelLayer;
};
}  // namespace FearEngine

#endif
