#ifndef FEARENGINE_RENDER_LAYER_H__
#define FEARENGINE_RENDER_LAYER_H__

#include <render/Camera.hpp>

namespace FearEngine::Render
{
//Todo remove camera or find a way to simplify camera to pass it's transforms and parameters to remove acces to frameBuffer modifications
class Layer
{
public:
	virtual void init() = 0;
	virtual void resize(int width, int height) = 0;
	virtual void preUpdate(Camera& cam) = 0;
	virtual void update(Camera& cam) = 0;
	virtual void postUpdate(Camera& cam) = 0;
};
}  // namespace FearEngine::Render

#endif
