#pragma once
#include <vector>
#include <memory>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class Entity;
class Renderer;

class Scene {
public:
    Scene() = default;

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

private:
    std::vector<std::shared_ptr<Entity>> entities;
};
