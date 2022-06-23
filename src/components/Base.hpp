#ifndef FEARENGINE_COMPONENTS_BASE_H__
#define FEARENGINE_COMPONENTS_BASE_H__

#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>



namespace FearEngine::Component
{
//Todo think about replacing that enum on hash map f.e to support user Components
enum ComponentType
{
	transform,
	tag,

	camera,
	noclipCameraController,

	renderable,
	light,
	directionalLight,

	invalid
};

#define GENCOMPONENTESSENTIALS(classType)                     \
	static ComponentType staticType() { return ComponentType::classType; } \
                                                                   \
	ComponentType type() const { return staticType(); }                \
                                                                   \
	const char* name() const { return #classType; }


struct Transform
{
	glm::vec3 pos;
	glm::vec3 rotation;
	glm::vec3 scale;

	Transform();

	glm::mat4 getTransformMatrix() const;

	GENCOMPONENTESSENTIALS(transform)
};

struct Tag
{
	std::string tag;

	Tag() = delete;
	Tag(const std::string& nameTag);

	GENCOMPONENTESSENTIALS(tag)
};
};	// namespace FearEngine::Component
#endif
