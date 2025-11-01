#include "game/Projectile.h"
#define GLM_ENABLE_EXPERIMENTAL

#include <glm/gtc/constants.hpp> // Per glm::pi<float>()
#include <glm/gtx/norm.hpp>      // Per glm::length2 (risolve E0135)
#include <cmath>                 // Per atan2

Projectile::Projectile(const std::string& name)
    : Entity(name) {
    // Il costruttore non ha bisogno di modifiche.
}

void Projectile::update(float deltaTime) {
    // 1. Movimento del Proiettile
    transform.translate(velocity * deltaTime);

    // 2. Orientamento Dinamico (allinea la rotazione alla velocità)
    // Usiamo length2 per evitare una costosa operazione di radice quadrata.
    if (glm::length2(velocity) > 0.0001f) {

        // Calcola l'angolo in radianti rispetto all'asse X positivo.
        float targetAngle = std::atan2(velocity.y, velocity.x);

        // Se la mesh Hermite del proiettile è orientata lungo l'asse Y (UP) a rotazione 0,
        // è necessaria una correzione di 90 gradi (pi/2) per allineare l'asse Y del modello
        // con l'angolo calcolato (che punta lungo l'asse X).
        transform.setRotation(targetAngle - glm::pi<float>() * 0.5f);
    }

    // 3. Diminuzione del Lifetime
    lifetime -= deltaTime;
    if (lifetime <= 0.0f) {
        setActive(false); // Verrà rimosso da Scene::update
    }

    // 4. Controllo Fuori Schermo (Opzionale ma Utile)
    const glm::vec2 pos = transform.getPosition();
    if (pos.x < -5.0f || pos.x > 5.0f || pos.y < -5.0f || pos.y > 5.0f) {
        setActive(false);
    }
}