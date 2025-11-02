#pragma once
#include <game/GameObject.h>
#include <vector>
#include <glm/glm.hpp>
#include "core/Engine.h"

class MeshManager;

class SpaceCleaner : public Entity {
private:
	float speed;
	glm::vec2 direction;
	unsigned int maxHealth = 6; // Nel caso in cui volessi incrementare la vitalità della navicella
	unsigned int health = 6;
	Engine* engine;
public:
	SpaceCleaner(const std::string& name = "SpaceCleaner", Engine* engine = nullptr);
	~SpaceCleaner() = default;

	void update(float deltaTime) override;

	void setSpeed(float s) {
		speed = s;
	}
	float getSpeed() const {
		return speed;
	}

	void setDirection(const glm::vec2& dir) {
		direction = dir;
	}

	void takeDamage() {
		health = health > 0 ? health - 1 : 0;
		engine->adjustScore(-100);
	}

	unsigned int getHealth() const { return health; }
	unsigned int getMaxHealth() const { return maxHealth; }
	void resetHealth() { health = maxHealth; }
};