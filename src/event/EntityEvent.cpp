#include "EntityEvent.hpp"

FearEngine::Events::EntityCreated::EntityCreated(FearEngine::Entity ent, const std::string_view& name)
 : updatedEntity(ent)
 , entityName(name)
{}

FearEngine::Entity FearEngine::Events::EntityCreated::getEntity() const { return updatedEntity; }

const std::string_view& FearEngine::Events::EntityCreated::getEntityName() const { return entityName; }

FearEngine::Events::EntityOnAttached::EntityOnAttached(FearEngine::Entity ent, Component::ComponentType entityComponentType)
 : updatedEntity(ent)
 , componentType(entityComponentType)
{}

FearEngine::Entity FearEngine::Events::EntityOnAttached::getEntity() const { return updatedEntity; }

FearEngine::Component::ComponentType FearEngine::Events::EntityOnAttached::getType() const { return componentType; }

FearEngine::Events::EntityRemoved::EntityRemoved(FearEngine::Entity ent)
 : updatedEntity(ent)
{}

FearEngine::Entity FearEngine::Events::EntityRemoved::getEntity() const { return updatedEntity; }

FearEngine::Events::EntityOnDettached::EntityOnDettached(FearEngine::Entity ent, Component::ComponentType entityComponentType)
 : updatedEntity(ent)
 , componentType(entityComponentType)
{}

FearEngine::Entity FearEngine::Events::EntityOnDettached::getEntity() const { return updatedEntity; }

FearEngine::Component::ComponentType FearEngine::Events::EntityOnDettached::getType() const { return componentType; }
