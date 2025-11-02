#include "scene/Scene.h"

#include <algorithm>

#include "game/GameObject.h"
#include "graphics/Renderer.h"
#include "game/Projectile.h"
#include "game/Obstacle.h"
#include "game/Collision.h"
#include "game/SpaceCleaner.h"

void Scene::addEntity(const std::shared_ptr<Entity>& entity) {
    entities.push_back(entity);
}

void Scene::update(float deltaTime) {
    for (auto& e : entities) {
        if (e && e->isActive())
            e->update(deltaTime);
    }
    // Rimuovi entità non attive (cleanup)
    entities.erase(std::remove_if(entities.begin(), entities.end(),
        [](const std::shared_ptr<Entity>& e) {
            return !e || !e->isActive();
        }), entities.end());
}

void Scene::render(Renderer& renderer, GLenum mode) {
    for (auto& e : entities) {
        if (e && e->isActive())
            renderer.drawEntityByInfo(*e, mode);
    }
}

void Scene::spawnProjectile(const glm::vec2& position, const glm::vec2& velocity, float lifetime, unsigned int meshId, unsigned int shaderId, const glm::vec4& color) {
    auto proj = std::make_shared<Projectile>("Projectile");
    proj->transform.setPosition(position);
    proj->setVelocity(velocity);
    proj->setLifetime(lifetime);

    // Aggiungi layer render (meshId e shaderId)
    SubMeshRenderInfo info(meshId, shaderId, color);
    info.localTransform = glm::mat4(1.0f);
    proj->addMeshLayer(info);

    addEntity(proj);
}

void Scene::clear() {
    entities.clear();
}

Scene::Scene() : randomEngine(std::random_device()()) {}


void Scene::updateSpawning(float deltaTime, unsigned int asteroidMeshId, unsigned int cometMeshId, unsigned int shaderId) {
    timeSinceLastSpawn += deltaTime;

    if (timeSinceLastSpawn >= spawnCooldown) {
        timeSinceLastSpawn = 0.0f;

        // 1. Calcola la posizione X casuale (in alto)
        float spawnX = xDist(randomEngine);
        glm::vec2 position(spawnX, 4.5f); // Spawna appena fuori schermo (Y=4.5)

        // 2. Scegli il tipo di ostacolo (50/50)
        Obstacle::Type type = (typeDist(randomEngine) < 0.5f) ? Obstacle::ASTEROID : Obstacle::COMET;

        unsigned int meshId = (type == Obstacle::ASTEROID) ? asteroidMeshId : cometMeshId;
        glm::vec4 color = (type == Obstacle::ASTEROID)
            ? glm::vec4(0.5f, 0.4f, 0.3f, 1.0f) // Grigio roccioso
            : glm::vec4(0.8f, 0.9f, 1.0f, 1.0f); // Azzurro ghiaccio

        auto obstacle = std::make_shared<Obstacle>((type == Obstacle::ASTEROID) ? "Asteroid" : "Comet", type);
        obstacle->transform.setPosition(position);

        // 3. Assegna proprietà
        float speed = (type == Obstacle::ASTEROID) ? 0.8f : 1.5f; // Comete più veloci
        obstacle->setVelocity(glm::vec2(0.0f, -speed));

        if (type == Obstacle::ASTEROID) {
            // Rotazione casuale per gli asteroidi
            std::uniform_real_distribution<float> rotDist(-1.5f, 1.5f);
            obstacle->setRotationSpeed(rotDist(randomEngine));
        }

        // 4. Aggiungi il layer di rendering
        SubMeshRenderInfo info(meshId, shaderId, color);
        info.localTransform = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f)); // Scala l'ostacolo
        obstacle->addMeshLayer(info);

        addEntity(obstacle);
    }
}

void Scene::checkCollisions() {
    std::vector<std::shared_ptr<Projectile>> projectiles;
    std::vector<std::shared_ptr<Obstacle>> obstacles;
    std::shared_ptr<Entity> playerRef = nullptr;

    // Ricaviamo tutti i vari riferimenti delle entità dalla scena
    for (const auto& e : entities) {
        if (e->isActive()) {
            if (auto p = std::dynamic_pointer_cast<Projectile>(e)) {
                projectiles.push_back(p);
            }
            else if(auto o = std::dynamic_pointer_cast<Obstacle>(e)) {
                obstacles.push_back(o);
            } else if (e->getName() == "SpaceCleaner") {
                playerRef = e;
            }
        }
    }

    // Verifico se ci sono collisioni tra proiettili e ostacoli
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
