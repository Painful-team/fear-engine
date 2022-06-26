#include "Scene.hpp"

#include "Entity.hpp"

#include "Engine.hpp"

#include <components/Base.hpp>

FearEngine::Entity FearEngine::Scene::createEntity(const std::string& name)
{
	Entity entity(entities.create(), this);

	Engine::logs()->log("Entity", "Entity {0} was created with name: {1}.", (uint32_t)entity.entity, name);

	entity.addComponent<Component::Transform>();
	entity.addComponent<Component::Tag>(name);

	auto evnt = Events::EntityCreated(entity, name);
	Engine::getDispatcher()->notify(&evnt);

	return entity;
}

void FearEngine::Scene::removeEntity(Entity& entity)
{
	entities.destroy(entity.entity);

	auto evnt = Events::EntityRemoved(entity);
	Engine::getDispatcher()->notify(&evnt);
	Engine::logs()->log("Entity", "Entity {0} was removed.", (uint32_t)entity.entity);

	entity.entity = entt::null;
	entity.scene = nullptr;
}

FearEngine::Entity FearEngine::Scene::getEntity(uint32_t enttNum) { return {static_cast<entt::entity>(enttNum), this}; }
