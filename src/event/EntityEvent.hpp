#ifndef FEARENGINE_EVENT_ENTITYEVENT_H__
#define FEARENGINE_EVENT_ENTITYEVENT_H__

#include <string>

#include <event/Event.hpp>
#include <components/Base.hpp>
#include <components/Entity_impl.hpp>

namespace FearEngine::Events
{
class EntityCreated: public Event
{
public:
	EntityCreated(Entity ent, const std::string_view& name);

	Entity getEntity() const;
	const std::string_view& getEntityName() const;

	GENCLASSESSETIALS(entityCreated, entities)
private:
	Entity updatedEntity;
	std::string_view entityName;
};

class EntityRemoved: public Event
{
public:
	EntityRemoved(Entity ent);

	Entity getEntity() const;

	GENCLASSESSETIALS(entityRemoved, entities)
private:
	Entity updatedEntity;
};

class EntityOnAttached: public Event
{
public:
	EntityOnAttached(Entity ent, Component::ComponentType entityComponentType);

	Entity getEntity() const;

	FearEngine::Component::ComponentType getType() const;

	GENCLASSESSETIALS(entityOnAttached, entities)
private:
	Entity updatedEntity;
	Component::ComponentType componentType;
};

class EntityOnDettached: public Event
{
public:
	EntityOnDettached(Entity ent, Component::ComponentType entityComponentType);

	Entity getEntity() const;

	FearEngine::Component::ComponentType getType() const;

	GENCLASSESSETIALS(entityOnDettached, entities)
private:
	Entity updatedEntity;
	Component::ComponentType componentType;
};
};	// namespace FearEngine::Events

#endif
