#include "game/SpaceCleaner.h"

#include "core/Engine.h"
#include "dependencies/earcut.hpp"

SpaceCleaner::SpaceCleaner(const std::string& name, Engine* engine)
	: Entity(name), speed(0.0f), direction(0.0f, 0.0f), engine(engine)
{}

void SpaceCleaner::update(float deltaTime)
{
	transform.setPosition(transform.getPosition() + direction * speed * deltaTime);
}