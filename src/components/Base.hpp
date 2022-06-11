#ifndef FEARENGINE_COMPONENTS_BASE_H__
#define FEARENGINE_COMPONENTS_BASE_H__

#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace FearEngine::Component
{
struct Transform
{
	glm::vec3 pos;
	glm::vec3 rotation;
	glm::vec3 scale;

	Transform();

	glm::mat4 getTransformMatrix() const;
};

struct Tag
{
	std::string tag;

	Tag() = delete;
	Tag(const std::string& nameTag);
};
};	// namespace FearEngine::Component
#endif
