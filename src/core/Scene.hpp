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
	void removeEntity(Entity& entity);

	Entity getEntity(uint32_t enttNum);

	template<class Component, class... Compss>
	decltype(auto) view()
	{
		return entities.view<Component, Compss...>();
	}

	template <class Component, class... Compss>
	decltype(auto) group()
	{
		return entities.group<Component, Compss...>();
	}

private:
	entt::registry entities;

private:
	friend class Entity;
	friend class Engine;
};
}  // namespace FearEngine
#endif
