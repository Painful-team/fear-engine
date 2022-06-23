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

	//TODO replace parameter on from uin32_t to Entity.
	template<class Component, class... Compss>
	decltype(auto) get(uint32_t entit)
	{
		return entities.get<Component, Compss...>((entt::entity)entit);
	}

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
