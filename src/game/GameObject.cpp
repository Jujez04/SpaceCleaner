#include "game/GameObject.h"
#include <glm/gtc/matrix_transform.hpp>

int Entity::nextId = 0;

const glm::mat4 TransformComponent::getModelMatrix() const
{
    glm::mat4 model(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f));
    model = glm::rotate(model, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(scale, 1.0f));
    return model;
}

void TransformComponent::setPosition(const glm::vec2& pos)
{
    position = pos;
}

void TransformComponent::setRotation(float rot)
{
    rotation = rot;
}

void TransformComponent::setScale(const glm::vec2& scl)
{
    scale = scl;
}

void TransformComponent::translate(const glm::vec2& offset)
{
    position += offset;
}

void TransformComponent::rotate(float angle)
{
    rotation += angle;
}

void TransformComponent::scaleBy(const glm::vec2& factor)
{
    scale *= factor;
}

Entity::Entity(const std::string& entityName)
    : id(nextId++), name(entityName), active(true)
{
    // Associa i componenti all'owner
    transform.owner = this;
    //mesh.owner = this;
    //color.owner = this;
}
