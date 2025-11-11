#include "game/Projectile.h"
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtc/constants.hpp> // glm::pi<float>()
#include <glm/gtx/norm.hpp>      // glm::length2
#include <cmath>                 // atan2

Projectile::Projectile(const std::string& name)
    : Entity(name) {
}

/**
 * @brief Aggiorna il movimento e la rotazione del proiettile.
 */
void Projectile::update(float deltaTime) {
    // 1. Movimento del proiettile
    transform.translate(velocity * deltaTime);

    // 2. Orientamento dinamico (rotazione nella direzione della velocità)
    if (glm::length2(velocity) > 0.0001f) {
        float targetAngle = std::atan2(velocity.y, velocity.x);

        // Se il modello è orientato lungo Y (UP) a rotazione 0, ruotiamo di -90°
        transform.setRotation(targetAngle - glm::pi<float>() * 0.5f);
    }

    // 3. Riduzione del lifetime
    lifetime -= deltaTime;
    if (lifetime <= 0.0f) {
        setActive(false);
        return;
    }

    // 4. Disattivazione se fuori schermo
    const glm::vec2 pos = transform.getPosition();
    if (pos.x < -5.0f || pos.x > 5.0f || pos.y < -5.0f || pos.y > 5.0f) {
        setActive(false);
    }
}
