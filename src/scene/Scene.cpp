#include "scene/Scene.h"
#include "game/GameObject.h"      // Full Entity definition
#include "graphics/Renderer.h"    // Full Renderer definition
#include "graphics/Shader.h"      // Full Shader definition (if needed)
#include "graphics/MeshManager.h"          // Full MeshManager definition

void Scene::addEntity(const std::shared_ptr<Entity>& e) {
    entities.push_back(e);
}

void Scene::update(float deltaTime) {
    for (auto& e : entities) {
        if (e->isActive()) {
            e->update(deltaTime);
        }
    }
}
