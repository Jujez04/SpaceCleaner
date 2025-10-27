#pragma once
#include <memory>

class Window;
class Renderer;

class Engine {
public:
    Engine();
    ~Engine();
    void init();

private:
    std::unique_ptr<Window> window;
    std::unique_ptr<Renderer> renderer;
    void gameLoop();
    void processInput();
    void update();
    void rendering();
};
