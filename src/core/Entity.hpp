#ifndef FEARENGINE_CORE_ENTITY_H__
#define FEARENGINE_CORE_ENTITY_H__

#include "Scene.hpp"

#include <entt/entt.hpp>

#include <type_traits>

#include <components/Base.hpp>

namespace FearEngine
{

class Entity
{
public:

	Entity();
	Entity(const Entity& other)
	 : entity(other.entity)
	 , scene(other.scene){}

	Entity(Entity&& other) noexcept = default;
	Entity& operator=(const Entity& other)
	{ 
		entity = other.entity;
		scene = other.scene;

		return *this;
	}

	Entity& operator=(Entity&& other) noexcept
	{
		entity = other.entity;
		scene = other.scene;

		entity = entt::null;
		scene = nullptr;

		return *this;
	}

	template<class Component>
	bool hasComponent()
	{
		return scene->entities.all_of<Component>(entity);
	}

	template<class Component, typename...Args>
	decltype(auto) addComponent(Args&&... args)
	{
		return scene->entities.emplace<Component>(entity, std::forward<Args>(args)...);
	}

	template <class Component>
	auto getComponent()
	{
		return scene->entities.get<Component>(entity);
	}

	template <class Component>
	auto removeComponent()
	{
		static_assert(std::is_same_v<Component, FearEngine::Component::Transform>, "Couldn't remove base Component");
		static_assert(std::is_same_v<Component, FearEngine::Component::Tag>, "Couldn't remove base Component");
		return scene->entities.remove<Component>();
	}

private:
	Entity(entt::entity entity, Scene* scene);

	entt::entity entity;
	Scene* scene;

	friend class Scene;
};
}  // namespace FearEngine
#endif
