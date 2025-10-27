#pragma once
#include <memory>

#ifndef APPLICATION_H
#define APPLICATION_H

class Window;
class Renderer;

class Application {
public:
    Application();
    ~Application();
    void init();

private:
    std::unique_ptr<Window> window;
    std::unique_ptr<Renderer> renderer;
    void gameLoop();
    void processInput();
    void update();
    void rendering();
};

#endif