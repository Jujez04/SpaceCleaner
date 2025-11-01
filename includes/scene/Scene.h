#pragma once
#include <vector>
#include <memory>
#include <random>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class Entity;
class Renderer;

class Scene {
public:
    Scene();
    ~Scene() = default;

    // Aggiunge una entità alla scena (ownership condivisa)
    void addEntity(const std::shared_ptr<Entity>& entity);

    // Aggiorna tutte le entità e rimuove quelle inactive
    void update(float deltaTime);

    // Render tramite renderer (usa Renderer::drawEntityByInfo)
    void render(Renderer& renderer, GLenum mode);

    // Spawning helper (usato da Engine per proiettili)
    void spawnProjectile(const glm::vec2& position, const glm::vec2& velocity, float lifetime, unsigned int meshId, unsigned int shaderId, const glm::vec4& color);

    // Remove all
    void clear();

    void updateSpawning(float deltaTime, unsigned int asteroidMeshId, unsigned int cometMeshId, unsigned int shaderId);

private:
    std::vector<std::shared_ptr<Entity>> entities;
    float spawnCooldown = 1.0f; // Ogni 1 secondo
    float timeSinceLastSpawn = 0.0f;

    // Generatore casuale
    std::mt19937 randomEngine;
    std::uniform_real_distribution<float> xDist = std::uniform_real_distribution<float>(-4.0f, 4.0f);
    std::uniform_real_distribution<float> typeDist = std::uniform_real_distribution<float>(0.0f, 1.0f);
};
