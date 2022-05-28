#ifndef FEARENGINE_RENDER_LAYER_H__
#define FEARENGINE_RENDER_LAYER_H__

#include <glm/glm.hpp>
#include <istream>
#include <sstream>
#include <string>
#include <vector>

namespace FearEngine::Render
{
class Layer
{
public:
	virtual void init() = 0;
	virtual void update() = 0;
};
}  // namespace FearEngine::Render

#endif
