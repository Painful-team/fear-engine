#ifndef FEARENGINE_RENDER_LAYER_H__
#define FEARENGINE_RENDER_LAYER_H__

#include <istream>
#include <sstream>
#include <string>
#include <glm/glm.hpp>
#include <vector>

namespace FearEngine::Render
{
class Layer
{
public:
	virtual void init() = 0;
	virtual void update() = 0;
};
}

#endif
