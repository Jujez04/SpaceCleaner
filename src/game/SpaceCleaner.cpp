#include "game/SpaceCleaner.h"
#include "core/Engine.h"
#include "dependencies/earcut.hpp"

SpaceCleaner::SpaceCleaner(const std::string& name, Engine* engine)
    : Entity(name), speed(0.0f), direction(0.0f, 0.0f), engine(engine) {}

/**
 * @brief Aggiorna la posizione della navicella in base alla direzione e velocità.
 */
void SpaceCleaner::update(float deltaTime)
{
    // Movimento lineare: posizione = posizione + direzione * velocità * tempo
    transform.setPosition(transform.getPosition() + direction * speed * deltaTime);
}

/**
 * @brief Riduce la salute del giocatore e aggiorna il punteggio.
 *
 * Usa l'engine per diminuire il punteggio di 100 punti.
 */
void SpaceCleaner::takeDamage()
{
    health = health > 0 ? health - 1 : 0;

    // Se disponibile, aggiorna il punteggio globale
    if (engine)
        engine->adjustScore(-100);
}
