#pragma once
#include "game/GameObject.h"
#include <glm/glm.hpp>

/**
 * @brief Rappresenta un proiettile sparato dal giocatore o da un nemico.
 *
 * Si muove nella direzione della sua velocità e si disattiva automaticamente
 * dopo un tempo limite o quando esce dallo schermo.
 */
class Projectile : public Entity {
public:
    explicit Projectile(const std::string& name = "Projectile");

    /**
     * @brief Aggiorna il movimento, rotazione e tempo di vita del proiettile.
     * @param deltaTime Tempo trascorso dall'ultimo frame
     */
    void update(float deltaTime) override;

    /// Imposta la velocità del proiettile
    void setVelocity(const glm::vec2& vel) { velocity = vel; }

    /// Imposta la durata del proiettile in secondi
    void setLifetime(float lt) { lifetime = lt; }

private:
    glm::vec2 velocity = glm::vec2(0.0f); ///< Velocità in 2D
    float lifetime = 5.0f;                ///< Durata in secondi
};
