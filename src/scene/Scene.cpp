#include "scene/Scene.h"
#include <algorithm>
#include "game/GameObject.h"
#include "graphics/Renderer.h"
#include "game/Projectile.h"
#include "game/Obstacle.h"
#include "game/Collision.h"
#include "game/SpaceCleaner.h"

/**
 * @brief Costruttore: inizializza il generatore casuale con un seed di sistema.
 */
Scene::Scene() : randomEngine(std::random_device()()) {}

/**
 * @brief Aggiunge una nuova entità attiva nella scena.
 */
void Scene::addEntity(const std::shared_ptr<Entity>& entity) {
    entities.push_back(entity);
}

/**
 * @brief Aggiorna tutte le entità e rimuove quelle inattive.
 *
 * Ogni entità riceve l'invocazione di `update(deltaTime)` per permettere
 * aggiornamenti interni (fisica, animazioni, vita residua, ecc.).
 *
 * Viene poi eseguita una pulizia tramite `std::remove_if` che elimina le entità
 * marcate come inattive.
 */
void Scene::update(float deltaTime) {
    for (auto& e : entities) {
        if (e && e->isActive())
            e->update(deltaTime);
    }

    // Pulizia delle entità non attive
    entities.erase(std::remove_if(entities.begin(), entities.end(),
        [](const std::shared_ptr<Entity>& e) {
            return !e || !e->isActive();
        }), entities.end());
}

/**
 * @brief Esegue il rendering di tutte le entità attive.
 */
void Scene::render(Renderer& renderer, GLenum mode) {
    for (auto& e : entities) {
        if (e && e->isActive())
            renderer.drawEntityByInfo(*e, mode);
    }
}

/**
 * @brief Crea un proiettile con parametri fisici e grafici specifici.
 *
 * Il proiettile viene configurato con:
 *  - posizione iniziale;
 *  - velocità e direzione;
 *  - durata di vita (lifetime);
 *  - mesh, shader e colore.
 *
 * Il rendering è affidato al `SubMeshRenderInfo`, che memorizza gli ID di rendering.
 */
void Scene::spawnProjectile(const glm::vec2& position, const glm::vec2& velocity,
    float lifetime, unsigned int meshId,
    unsigned int shaderId, const glm::vec4& color) {
    auto proj = std::make_shared<Projectile>("Projectile");
    proj->transform.setPosition(position);
    proj->setVelocity(velocity);
    proj->setLifetime(lifetime);

    SubMeshRenderInfo info(meshId, shaderId, color);
    info.localTransform = glm::mat4(1.0f);
    proj->addMeshLayer(info);

    addEntity(proj);
}

/**
 * @brief Rimuove tutte le entità attualmente nella scena.
 */
void Scene::clear() {
    entities.clear();
}

/**
 * @brief Genera nuovi ostacoli (asteroidi/comete) in modo casuale e periodico.
 *
 * Ogni `spawnCooldown` secondi, un nuovo oggetto viene creato:
 *  - la **posizione X** è casuale tra -1 e 1;
 *  - la **posizione Y** è fissa sopra lo schermo;
 *  - il **tipo** è deciso casualmente (50% asteroide / 50% cometa);
 *  - le **comete** sono più veloci, mentre gli **asteroidi** hanno una rotazione casuale;
 *  - viene applicata una trasformazione di scala per il rendering.
 *
 * L’approccio statistico crea una varietà visiva e dinamica nel gameplay.
 */
void Scene::updateSpawning(float deltaTime, unsigned int asteroidMeshId,
    unsigned int cometMeshId, unsigned int shaderId) {
    timeSinceLastSpawn += deltaTime;

    if (timeSinceLastSpawn >= spawnCooldown) {
        timeSinceLastSpawn = 0.0f;

        float spawnX = xDist(randomEngine);
        glm::vec2 position(spawnX, 1.25f); // appena fuori dallo schermo

        // Determina tipo di oggetto
        Obstacle::Type type = (typeDist(randomEngine) < 0.5f)
            ? Obstacle::ASTEROID : Obstacle::COMET;

        unsigned int meshId = (type == Obstacle::ASTEROID) ? asteroidMeshId : cometMeshId;
        glm::vec4 color = (type == Obstacle::ASTEROID)
            ? glm::vec4(0.5f, 0.4f, 0.3f, 1.0f)
            : glm::vec4(0.8f, 0.9f, 1.0f, 1.0f);

        auto obstacle = std::make_shared<Obstacle>(
            (type == Obstacle::ASTEROID) ? "Asteroid" : "Comet", type);
        obstacle->transform.setPosition(position);

        float speed = (type == Obstacle::ASTEROID) ? 0.8f : 1.5f;
        obstacle->setVelocity(glm::vec2(0.0f, -speed));

        // Rotazione casuale per asteroidi
        if (type == Obstacle::ASTEROID) {
            std::uniform_real_distribution<float> rotDist(-1.5f, 1.5f);
            obstacle->setRotationSpeed(rotDist(randomEngine));
        }

        SubMeshRenderInfo info(meshId, shaderId, color);
        info.localTransform = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
        obstacle->addMeshLayer(info);

        addEntity(obstacle);
    }
}

/**
 * @brief Verifica e gestisce le collisioni tra entità.
 *
 * - I proiettili vengono controllati contro ogni ostacolo tramite le rispettive AABB.
 * - Le collisioni AABB sono calcolate nel sistema di coordinate del mondo.
 * - Quando una collisione è rilevata, le entità coinvolte vengono disattivate.
 * - Se il player collide con un ostacolo, subisce danno (chiamata `takeDamage()`).
 */
void Scene::checkCollisions() {
    std::vector<std::shared_ptr<Projectile>> projectiles;
    std::vector<std::shared_ptr<Obstacle>> obstacles;
    std::shared_ptr<Entity> playerRef = nullptr;

    // Classifica entità per tipo dinamico
    for (const auto& e : entities) {
        if (e->isActive()) {
            if (auto p = std::dynamic_pointer_cast<Projectile>(e)) {
                projectiles.push_back(p);
            }
            else if (auto o = std::dynamic_pointer_cast<Obstacle>(e)) {
                obstacles.push_back(o);
            }
            else if (e->getName() == "SpaceCleaner") {
                playerRef = e;
            }
        }
    }

    // Proiettili vs ostacoli
    for (const auto& proj : projectiles) {
        if (proj->isActive()) {
            AABB projBox = proj->getAABB();
            for (const auto& obs : obstacles) {
                if (obs->isActive()) {
                    AABB obsBox = obs->getAABB();
                    if (projBox.isColliding(obsBox)) {
                        proj->setActive(false);
                        obs->setActive(false);
                        numCollisions++;
                    }
                }
            }
        }
    }

    // Player vs ostacoli
    if (playerRef) {
        AABB playerBox = playerRef->getAABB();
        for (const auto& obs : obstacles) {
            if (obs->isActive()) {
                AABB obsBox = obs->getAABB();
                if (playerBox.isColliding(obsBox)) {
                    obs->setActive(false);
                    if (auto playerCasted = std::dynamic_pointer_cast<SpaceCleaner>(playerRef)) {
                        playerCasted->takeDamage();
                    }
                }
            }
        }
    }
}

/**
 * @brief Restituisce il numero di collisioni e azzera il contatore.
 */
int Scene::getCollisions() {
    int temp = numCollisions;
    numCollisions = 0;
    return temp;
}
