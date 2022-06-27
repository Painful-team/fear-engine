#ifndef FEARENGINE_RENDER_LAYER_H__
#define FEARENGINE_RENDER_LAYER_H__

#include <components/CameraComponent.hpp>


namespace FearEngine::Render
{
using debugProperty = short;
//Todo remove camera or find a way to simplify camera to pass it's transforms and parameters to remove acces to frameBuffer modifications
class Layer
{
public:
	virtual errorCode init() = 0;
	virtual void resize(int width, int height) = 0;
	virtual void preUpdate(Component::Camera& cam) = 0;
	virtual void update(Component::Camera& cam) = 0;
	virtual void postUpdate(Component::Camera& cam) = 0;
	virtual debugProperty debugProperty() const = 0;
};
}  // namespace FearEngine::Render

#endif
