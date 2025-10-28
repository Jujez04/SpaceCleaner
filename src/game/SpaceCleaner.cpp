#include "game/SpaceCleaner.h"

SpaceCleaner::SpaceCleaner(const std::string& name)
	: Entity(name), speed(0.0f), direction(0.0f, 0.0f)
{}

void SpaceCleaner::update(float deltaTime)
{
	transform.setPosition(transform.getPosition() + direction * speed * deltaTime);
}

void SpaceCleaner::generateHermiteMesh(const std::vector<glm::vec2>& controlPoints, int segment)
{
}

void SpaceCleaner::onCollision(Entity* other)
{
	// Handle collision with other entity
}