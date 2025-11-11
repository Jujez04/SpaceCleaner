#pragma once
#include "game/GameObject.h"
#include <glm/glm.hpp>

/**
 * @brief Rappresenta un ostacolo nel gioco (asteroide o cometa).
 *
 * Gli ostacoli si muovono nello spazio e possono ruotare.
 * Quando escono dallo schermo, vengono disattivati.
 */
class Obstacle : public Entity {
public:
    /// Tipi di ostacolo disponibili
    enum Type { ASTEROID, COMET };

    /**
     * @brief Costruttore.
     * @param name Nome dell'entità
     * @param type Tipo dell'ostacolo
     */
    Obstacle(const std::string& name, Type type);

    /**
     * @brief Aggiorna lo stato dell'ostacolo.
     * @param deltaTime Tempo trascorso dall'ultimo frame (in secondi)
     */
    void update(float deltaTime) override;

    /// Imposta la velocità dell'ostacolo
    void setVelocity(const glm::vec2& vel) { velocity = vel; }

    /// Imposta la velocità di rotazione (solo per asteroidi)
    void setRotationSpeed(float speed) { rotationSpeed = speed; }

private:
    glm::vec2 velocity = glm::vec2(0.0f, -0.5f);  ///< Velocità verso il basso
    float rotationSpeed = 0.0f;                   ///< Velocità di rotazione
    Type type;                                   ///< Tipo di ostacolo
};
