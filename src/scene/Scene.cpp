#include "scene/Scene.h"

#include <algorithm>

#include "game/GameObject.h"
#include "graphics/Renderer.h"
#include "game/Projectile.h"

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
