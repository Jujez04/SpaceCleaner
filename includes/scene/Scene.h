#pragma once
#include <vector>
#include <memory>
#include <random>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class Entity;
class Renderer;

/**
 * @class Scene
 * @brief Gestisce l'insieme di entità attive nel mondo di gioco e la loro interazione.
 *
 * La classe `Scene` rappresenta un contenitore logico per tutte le entità attualmente presenti
 * (player, proiettili, ostacoli, ecc.). È responsabile di:
 *  - aggiornare le entità nel tempo;
 *  - gestire lo spawning periodico di oggetti casuali (asteroidi/comete);
 *  - effettuare il rendering di ciascuna entità tramite il `Renderer`;
 *  - verificare le collisioni tra entità (proiettile-ostacolo e player-ostacolo);
 *  - mantenere un contatore delle collisioni avvenute.
 */
class Scene {
public:
    Scene();
    ~Scene() = default;

    /**
     * @brief Aggiunge una nuova entità alla scena.
     * @param entity Puntatore condiviso all'entità da aggiungere.
     */
    void addEntity(const std::shared_ptr<Entity>& entity);

    /**
     * @brief Aggiorna tutte le entità attive e rimuove quelle non più valide.
     * @param deltaTime Intervallo di tempo trascorso (in secondi).
     */
    void update(float deltaTime);

    /**
     * @brief Esegue il rendering di tutte le entità attive nella scena.
     * @param renderer Riferimento al renderer globale.
     * @param mode Modalità OpenGL (es. GL_TRIANGLES, GL_LINE_LOOP, ecc.).
     */
    void render(Renderer& renderer, GLenum mode);

    /**
     * @brief Genera e aggiunge un proiettile alla scena.
     *
     * Il proiettile viene configurato con posizione, velocità, durata e dati di rendering.
     * È pensato per essere richiamato dal motore di gioco o dal giocatore.
     */
    void spawnProjectile(const glm::vec2& position, const glm::vec2& velocity,
        float lifetime, unsigned int meshId,
        unsigned int shaderId, const glm::vec4& color);

    /**
     * @brief Rimuove tutte le entità dalla scena.
     */
    void clear();

    /**
     * @brief Gestisce lo spawning automatico e casuale di ostacoli (asteroidi o comete).
     *
     * Usa distribuzioni uniformi per decidere la posizione e il tipo di ostacolo.
     * Gli oggetti vengono generati periodicamente in alto allo schermo e cadono verso il basso.
     *
     * @param deltaTime Tempo trascorso dall'ultimo frame.
     * @param asteroidMeshId ID della mesh degli asteroidi.
     * @param cometMeshId ID della mesh delle comete.
     * @param shaderId ID dello shader usato per disegnare gli oggetti.
     */
    void updateSpawning(float deltaTime, unsigned int asteroidMeshId,
        unsigned int cometMeshId, unsigned int shaderId);

    /**
     * @brief Verifica le collisioni tra entità rilevanti nella scena.
     *
     * - **Proiettili vs Ostacoli**: entrambi vengono disattivati e incrementa il contatore.
     * - **Player vs Ostacoli**: l'ostacolo viene distrutto, il player subisce danno.
     */
    void checkCollisions();

    /**
     * @brief Restituisce tutte le entità correnti nella scena.
     */
    std::vector<std::shared_ptr<Entity>> getEntities() const { return entities; }

    /**
     * @brief Restituisce il numero di collisioni rilevate dall'ultimo frame.
     * Il contatore viene azzerato dopo la lettura.
     */
    int getCollisions();

private:
    std::vector<std::shared_ptr<Entity>> entities; ///< Contenitore di tutte le entità attive.
    float spawnCooldown = 1.0f; ///< Tempo minimo tra due spawn consecutivi.
    float timeSinceLastSpawn = 0.0f; ///< Timer interno per lo spawning.
    int numCollisions = 0; ///< Contatore temporaneo di collisioni rilevate.

    // Generatore casuale per spawning
    std::mt19937 randomEngine;
    std::uniform_real_distribution<float> xDist = std::uniform_real_distribution<float>(-1.0f, 1.0f);
    std::uniform_real_distribution<float> typeDist = std::uniform_real_distribution<float>(0.0f, 1.0f);
};
