#pragma once
#include <memory>

class Window;
class Renderer;
class Camera;
class SpaceCleaner;
class Scene;

class Engine {
public:
    Engine();
    ~Engine();
    void init();
    void gameLoop();
private:
    std::unique_ptr<Window> window;
    std::unique_ptr<Renderer> renderer;
    std::unique_ptr<Camera> camera;
    std::unique_ptr<Scene> scene;
    std::unique_ptr<SpaceCleaner> player;
    unsigned int defaultShaderId = 0;
    unsigned int projectileMeshId = 0;
    double lastFrameTime = 0.0;
    float fireCooldown = 0.2f;
    float timeSinceLastShot = 0.0f;
    unsigned int asteroidMeshId = 0;
    unsigned int cometMeshId = 0;   

    void processInput();
    void update(float deltaTime);
    void rendering();
};
