#include "application/Application.h"

#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdexcept>

#include "application/Window.h"
#include "graphics/Vertex.h"
#include "graphics/Shader.h"

Application::Application() {}

Application::~Application() {
    glfwTerminate();
}

void Application::processInput() {
    // codice input
}

void Application::update() {
    // codice update
}

void Application::rendering() {
    renderer->clear();
	renderer->draw(3); // Disegna un triangolo con 3 vertici
	window->updateWindow();
}

void Application::gameLoop() {
    while (window->windowIsOpen()) {
        window->pollEvents();
        processInput();
        update();
        rendering();
    }
}

void Application::init() {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = std::make_unique<Window>(800, 600);

    // Initialize renderer with vertex data
    float vertices[] = {
         0.0f,  0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f
    };
    renderer = std::make_unique<Renderer>(vertices, sizeof(vertices));

    gameLoop();
}