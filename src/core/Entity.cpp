#include "Entity.hpp"


FearEngine::Entity::Entity()
 : scene(nullptr)
 , entity(entt::null)
{}

FearEngine::Entity::Entity(entt::entity ent, Scene* scn)
 : entity(ent)
 , scene(scn)
{}
