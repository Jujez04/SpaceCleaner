#pragma once
#include "game/GameObject.h"
#include <glm/glm.hpp>

class Projectile : public Entity {
public:
    Projectile(const std::string& name = "Projectile");

    void update(float deltaTime) override;

    void setVelocity(const glm::vec2& vel) { velocity = vel; }
    void setLifetime(float lt) { lifetime = lt; }

private:
    glm::vec2 velocity = glm::vec2(0.0f);
    float lifetime = 5.0f; // default 5s
};
