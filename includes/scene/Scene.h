#pragma once
#include <vector>
#include <memory>

// Forward declarations only
class Entity;
class Renderer;
class Shader;
class MeshManager;  // Forward declaration

class Scene {
private:
    std::vector<std::shared_ptr<Entity>> entities;
public:
    Scene() = default;
    ~Scene() = default;

    void addEntity(const std::shared_ptr<Entity>& e);
    void update(float deltaTime);
};