#pragma once
#include <memory>

class Window;
class Renderer;
class Camera;

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

    void processInput();
    void update();
    void rendering();
};
