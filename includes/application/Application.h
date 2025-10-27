#pragma once
#include <memory>

#ifndef APPLICATION_H
#define APPLICATION_H

class Window;

class Application {
public:
    Application();
    ~Application();
    void run();

private:
    std::unique_ptr<Window> window;
    void gameLoop();
    void processInput();
    void update();
    void rendering();
};

#endif