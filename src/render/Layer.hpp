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

struct Vertex
{
	glm::vec3 position;
	glm::vec2 texcoord;
	glm::vec3 normal;
};

struct VertRef
{
	VertRef(int v, int vt, int vn) : v(v), vt(vt), vn(vn) {}
	int v, vt, vn;
};

std::vector< Vertex > LoadOBJ(std::istream& in);
}

#endif
