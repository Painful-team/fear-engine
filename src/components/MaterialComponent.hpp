#ifndef FEARENGINE_COMPONENTS_MATERIALCOMPONENT_H__
#define FEARENGINE_COMPONENTS_MATERIALCOMPONENT_H__

#include <memory>

#include <glm/vec3.hpp>

#include <cache/MaterialResource.hpp>

#include "Base.hpp"

namespace FearEngine::Component
{
struct Renderable
{
	std::shared_ptr<Cache::Resource> mesh;

	//Think about replacing vector on array for reducing allocation
	std::vector<std::shared_ptr<Cache::Material>> materials;

	GENCOMPONENTESSENTIALS(renderable)
};

struct Light
{
	bool isPoint;

	glm::vec3 pos;
	glm::vec3 dir;

	float constant;
	float linear;
	float quadratic;

	float cutOff;
	float outerCutOff;

	glm::vec3 lightColor;

	GENCOMPONENTESSENTIALS(light)
};

struct DirectionalLight
{
	glm::vec3 dir;
	glm::vec3 lightColor;

	GENCOMPONENTESSENTIALS(directionalLight)
};
}

#endif	// !FEARENGINE_COMPONENTS_MATERIALCOMPONENT_H__
