#include "game/Obstacle.h"

Obstacle::Obstacle(const std::string& name, Type type)
    : Entity(name), type(type) {
}

void Obstacle::update(float deltaTime) {
    // 1. Movimento: sposta l'ostacolo in base alla velocità
    transform.translate(velocity * deltaTime);

    // 2. Rotazione (per gli asteroidi)
    if (rotationSpeed != 0.0f) {
        transform.rotate(rotationSpeed * deltaTime);
    }

    // 3. Cleanup: Rimuove l'ostacolo quando esce dallo schermo (sotto)
    const glm::vec2 pos = transform.getPosition();
    if (pos.y < -5.0f || pos.x < -5.0f || pos.x > 5.0f) {
        setActive(false);
    }
}