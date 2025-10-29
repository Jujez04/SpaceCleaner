#include "core/Engine.h"

#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdexcept>

#include "core/Window.h"
#include "graphics/Vertex.h"
#include "graphics/Shader.h"
#include "math/Hermite.h"

Engine::Engine() {}

Engine::~Engine() {
    glfwTerminate();
}

void Engine::processInput() {
    // codice input
}

void Engine::update() {
    // codice update
}

void Engine::rendering() {
    renderer->clear();
	renderer->draw();
	window->updateWindow();
}

void Engine::gameLoop() {
    while (window->windowIsOpen()) {
        window->pollEvents();
        processInput();
        update();
        rendering();
    }
}

void Engine::init() {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    window = std::make_unique<Window>(800, 600);

    std::vector<glm::vec2> controlPoints = {
        glm::vec2(0.0f, 0.6f),    // punta superiore (naso)
        glm::vec2(0.1f, 0.35f),   // bordo destro fusoliera
        glm::vec2(0.35f, 0.15f),  // ala destra superiore
        glm::vec2(0.6f, -0.05f),  // estremità ala destra
        glm::vec2(0.35f, -0.2f),  // ala destra inferiore
        glm::vec2(0.15f, -0.4f),  // fusoliera bassa destra
        glm::vec2(0.1f, -0.55f),  // motore destro
        glm::vec2(0.0f, -0.6f),   // coda centrale
        glm::vec2(-0.1f, -0.55f), // motore sinistro
        glm::vec2(-0.15f, -0.4f), // fusoliera bassa sinistra
        glm::vec2(-0.35f, -0.2f), // ala sinistra inferiore
        glm::vec2(-0.6f, -0.05f), // estremità ala sinistra
        glm::vec2(-0.35f, 0.15f), // ala sinistra superiore
        glm::vec2(-0.1f, 0.35f),  // bordo sinistro fusoliera
        glm::vec2(0.0f, 0.6f)     // chiusura del loop
    };

    auto tangents = HermiteUtility::calculateTangents(controlPoints, 0.2f);
    auto spaceshipOutline = HermiteUtility::generateClosedHermiteCurve(controlPoints, tangents, 60);

    std::vector<float> vertices;
    for (auto& p : spaceshipOutline) {
        vertices.push_back(p.x);
        vertices.push_back(p.y);
        vertices.push_back(0.0f);
        vertices.push_back(0.0f);    // R
        vertices.push_back(0.4f);    // G
        vertices.push_back(0.8f);    // B
    }
    renderer = std::make_unique<Renderer>(
        vertices.data(),
        vertices.size() * sizeof(float),
        static_cast<unsigned int>(spaceshipOutline.size())
    );


    gameLoop();
}