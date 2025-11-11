#pragma once
#include <game/GameObject.h>
#include <vector>
#include <glm/glm.hpp>
#include "core/Engine.h"

class MeshManager;

/**
 * @brief Rappresenta la navicella controllata dal giocatore ("SpaceCleaner").
 *
 * Gestisce movimento, direzione, velocità e salute. È collegata a un oggetto
 * Engine per modificare il punteggio in caso di danni.
 */
class SpaceCleaner : public Entity {
private:
    float speed;                      ///< Velocità di movimento
    glm::vec2 direction;              ///< Direzione normalizzata del movimento
    unsigned int maxHealth = 6;       ///< Punti vita massimi
    unsigned int health = 6;          ///< Punti vita correnti
    Engine* engine;                   ///< Puntatore all'engine per gestione punteggio

public:
    /**
     * @brief Costruttore della navicella del giocatore.
     * @param name Nome identificativo dell'entità
     * @param engine Puntatore all'istanza di Engine (per aggiornare punteggi o stato di gioco)
     */
    SpaceCleaner(const std::string& name = "SpaceCleaner", Engine* engine = nullptr);

    ~SpaceCleaner() override = default;

    /**
     * @brief Aggiorna la posizione in base alla direzione e velocità correnti.
     * @param deltaTime Tempo trascorso dall’ultimo frame
     */
    void update(float deltaTime) override;

    /// Imposta la velocità della navicella
    void setSpeed(float s) { speed = s; }

    /// Restituisce la velocità corrente
    float getSpeed() const { return speed; }

    /// Imposta la direzione di movimento (deve essere normalizzata)
    void setDirection(const glm::vec2& dir) { direction = dir; }

    /**
     * @brief Applica danno alla navicella, riducendo la salute.
     *
     * Se la salute scende sotto zero, viene mantenuta a zero.
     * Inoltre riduce il punteggio del giocatore tramite l'engine.
     */
    void takeDamage();

    /// Restituisce la salute attuale
    unsigned int getHealth() const { return health; }

    /// Restituisce la salute massima
    unsigned int getMaxHealth() const { return maxHealth; }

    /// Ripristina la salute al valore massimo
    void resetHealth() { health = maxHealth; }
};
