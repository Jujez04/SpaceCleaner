#include "game/SpaceCleaner.h"


#include "dependencies/earcut.hpp"

SpaceCleaner::SpaceCleaner(const std::string& name)
	: Entity(name), speed(0.0f), direction(0.0f, 0.0f)
{}

void SpaceCleaner::update(float deltaTime)
{
	transform.setPosition(transform.getPosition() + direction * speed * deltaTime);
}