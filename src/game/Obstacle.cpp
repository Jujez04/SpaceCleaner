#include "game/Obstacle.h"

Obstacle::Obstacle(const std::string& name, Type type)
    : Entity(name), type(type) {
}

/**
 * @brief Aggiorna la posizione e la rotazione dell'ostacolo.
 * Se esce dai limiti dello schermo, viene disattivato.
 */
void Obstacle::update(float deltaTime) {
    // Movimento lineare
    transform.translate(velocity * deltaTime);

    // Rotazione solo per asteroidi
    if (type == ASTEROID && rotationSpeed != 0.0f) {
        transform.rotate(rotationSpeed * deltaTime);
    }

    // Controllo fuori schermo
    const glm::vec2 pos = transform.getPosition();
    if (pos.y < -5.0f || pos.x < -5.0f || pos.x > 5.0f) {
        setActive(false);
    }
}
