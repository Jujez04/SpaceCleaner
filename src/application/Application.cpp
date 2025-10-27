#include "application/Application.h"
#include "scene/Window.h"
#include <memory>

Application::Application() {}

Application::~Application() {}

void Application::processInput() {
    // codice input
}

void Application::update() {
    // codice update
}

void Application::rendering() {
    // codice rendering
}

void Application::gameLoop() {
    while (window->windowIsOpen()) {
        window->pollEvents();
        processInput();
        update();
        rendering();
    }
}

void Application::run() {
    window = std::make_unique<Window>(800, 600);
    gameLoop();
}
