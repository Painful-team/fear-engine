#ifndef FEARENGINE_CORE_SCENE_H__
#define FEARENGINE_CORE_SCENE_H__

#include <entt/entt.hpp>

#include <string>

namespace FearEngine
{
class Entity;

class Scene
{
public:
	Scene() = default;

	Entity createEntity(const std::string& name);

	template<class Component, class...Compss>
	auto view()
	{
		return entities.view<Component, Compss...>();
	}

	template <class Component, class... Compss>
	auto group() { return entities.group<Component, Compss>(); }

private:
	entt::registry entities;

private:
	friend class Entity;
};
}  // namespace FearEngine
#endif
