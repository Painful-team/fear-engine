#ifndef FEARENGINE_COMPONENTS_ENTITY_IMPL_H__
#define FEARENGINE_COMPONENTS_ENTITY_IMPL_H__

#include <entt/entt.hpp>

#include <type_traits>

#include <components/Base.hpp>

#include <core/Engine.hpp>
#include <core/Scene.hpp>

namespace FearEngine::Events
{
class EntityCreated;
class EntityOnAttached;
class EntityOnDettached;
class EntityRemoved;
};

namespace FearEngine
{
class Entity
{
public:
	Entity();
	Entity(const Entity& other);

	Entity(Entity&& other) noexcept;
	Entity& operator=(const Entity& other);
	Entity& operator=(Entity&& other) noexcept;

	bool isValid() const;

	template<class Component>
	bool hasComponent()
	{
		return scene->entities.all_of<Component>(entity);
	}

	bool operator==(uint32_t entityNum) const;
	bool operator==(const Entity& ent) const;

	bool operator!=(uint32_t entityNum) const;
	bool operator!=(const Entity& ent) const;

	template<class Component, typename...Args>
	decltype(auto) addComponent(Args&&... args)
	{
		auto&& component = scene->entities.emplace<Component>(entity, std::forward<Args>(args)...);
		auto evnt = Events::EntityOnAttached(*this, component.type());
		Engine::getDispatcher()->notify(&evnt);

		Engine::logs()->log("Entity", "{0} component was added to entity {1}.", component.name(), entity);
		return component;
	}

	template <class Component, typename... Args>
	decltype(auto) getComponent()
	{
		return scene->entities.get<Component, Args...>(entity);
	}

	template <class Component>
	decltype(auto) removeComponent()
	{
		static_assert(std::is_same_v<Component, FearEngine::Component::Transform>, "Couldn't remove base Component");
		static_assert(std::is_same_v<Component, FearEngine::Component::Tag>, "Couldn't remove base Component");

		//Todo when event system will be rewritten to event queue it has to be rewritten too
		auto evnt = Events::EntityRemoved(*this, Component::staticType());
		Engine::getDispatcher()->notify(&evnt);
		auto&& component = scene->entities.remove<Component>(entity);

		Engine::logs()->log("Entity", "{0} component was removed from entity {1}.", component.name(), entity);

		return component;
	}

private:
	Entity(entt::entity entity, Scene* scene);

	entt::entity entity;
	Scene* scene;

	friend class Scene;
};
}  // namespace FearEngine

#endif
