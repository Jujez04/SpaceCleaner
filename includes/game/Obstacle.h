#pragma once
#include "game/GameObject.h"
#include <glm/glm.hpp>

class Obstacle : public Entity {
public:
    // Il tipo aiuta per la collisione
    enum Type { ASTEROID, COMET };

    Obstacle(const std::string& name, Type type);

    void update(float deltaTime) override;

    void setVelocity(const glm::vec2& vel) { velocity = vel; }
    void setRotationSpeed(float speed) { rotationSpeed = speed; }

private:
    glm::vec2 velocity = glm::vec2(0.0f, -0.5f); // Velocità di default verso il basso
    float rotationSpeed = 0.0f; // Velocità di rotazione (per gli asteroidi)
    Type type;
};