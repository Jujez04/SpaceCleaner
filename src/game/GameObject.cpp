#include "game/GameObject.h"

TransformComponent::~TransformComponent()
{
}

glm::mat4 TransformComponent::getModelMatrix()
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(position, 0.0f));
	model = glm::rotate(model, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(scale, 1.0f));
	return model;
}

Entity::Entity(const std::string& entityName) {
	name = entityName;
}

void Entity::onCollision(Entity* other)
{

}

