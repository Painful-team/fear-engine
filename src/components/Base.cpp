#include "Base.hpp"

glm::mat4 FearEngine::Component::Transform::getTransformMatrix() const
{
	glm::mat4 rotationMatrix = glm::toMat4(glm::quat(glm::radians(rotation)));

	return glm::translate(glm::mat4(1.0f), pos) * rotationMatrix * glm::scale(glm::mat4(1.0f), scale);
}

FearEngine::Component::Transform::Transform()
 : pos(0, 0, 0)
 , rotation(0, 0, 0)
 , scale(1, 1, 1)
{}

FearEngine::Component::Tag::Tag(const std::string& nameTag)
 : tag(nameTag)
{}
