#include "Scene.hpp"

#include "Entity.hpp"

#include <components/Base.hpp>

FearEngine::Entity FearEngine::Scene::createEntity(const std::string& name)
{ 
	Entity entity(entities.create(), this);
	entity.addComponent<Component::Transform>();
	entity.addComponent<Component::Tag>(name);

	return entity;
}

void FearEngine::Scene::removeEntity(Entity& entity) 
{ 
	entities.destroy(entity.entity); 

	entity.entity = entt::null;
	entity.scene = nullptr;
}
