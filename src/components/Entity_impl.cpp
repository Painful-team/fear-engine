#include "Entity_impl.hpp"

FearEngine::Entity::Entity()
 : scene(nullptr)
 , entity(entt::null)
{}

FearEngine::Entity::Entity(const Entity& other) { *this = other; }

FearEngine::Entity::Entity(Entity&& other) noexcept { *this = std::move(other); }

FearEngine::Entity& FearEngine::Entity::operator=(const Entity& other)
{
	entity = other.entity;
	scene = other.scene;

	return *this;
}

FearEngine::Entity& FearEngine::Entity::operator=(Entity&& other) noexcept
{
	entity = other.entity;
	scene = other.scene;

	other.entity = entt::null;
	other.scene = nullptr;

	return *this;
}

bool FearEngine::Entity::isValid() const { return scene != nullptr && scene->entities.valid(entity); }

bool FearEngine::Entity::operator==(uint32_t entityNum) const { return static_cast<uint32_t>(entity) == entityNum; }

bool FearEngine::Entity::operator==(const Entity& ent) const { return entity == ent.entity; }

bool FearEngine::Entity::operator!=(uint32_t entityNum) const { return !(*this == entityNum); }

bool FearEngine::Entity::operator!=(const Entity& ent) const { return !(*this == ent); }

FearEngine::Entity::Entity(entt::entity ent, Scene* scn)
 : entity(ent)
 , scene(scn)
{}
