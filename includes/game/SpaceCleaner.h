#pragma once
#include <game/GameObject.h>
#include <vector>

class MeshManager;

class SpaceCleaner : public Entity {
private:
	float speed;
	glm::vec2 direction;

public:
	SpaceCleaner(const std::string& name = "SpaceCleaner");
	~SpaceCleaner() = default;

	void update(float deltaTime) override;

	void setSpeed(float s) {
		speed = s;
	}
	float getSpeed() const {
		return speed;
	}

	unsigned int generateHermiteMesh(const std::vector<glm::vec2>& controlPoints, int segment = 40);


	void setDirection(const glm::vec2& dir) {
		direction = dir;
	}
};