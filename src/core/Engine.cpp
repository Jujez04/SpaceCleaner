#include "core/Engine.h"

#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdexcept>

#include "core/Window.h"
#include "graphics/Vertex.h"
#include "graphics/Shader.h"
#include "math/Hermite.h"
#include "graphics/Camera.h"
#include "graphics/Mesh.h"

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
    renderer->setCamera(camera->getViewMatrix(), camera->getProjectionMatrix());
    renderer->drawAll(GL_LINE_STRIP);
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
    int width = 800;
    int height = 600;
    window = std::make_unique<Window>(width, height);

    std::vector<glm::vec2> controlPoints = {
        glm::vec2(-0.231771f,  0.099537f),
        glm::vec2(-0.282552f, -0.129630f),
        glm::vec2(-0.194010f, -0.115741f),
        glm::vec2(-0.128906f, -0.312500f),
        glm::vec2(-0.084635f, -0.307870f),
        glm::vec2(-0.067708f, -0.412037f),
        glm::vec2(-0.023438f, -0.317130f),
        glm::vec2(0.029948f, -0.307870f),
        glm::vec2(0.088542f, -0.087963f),
        glm::vec2(0.174479f, -0.074074f),
        glm::vec2(0.083333f,  0.134259f),
        glm::vec2(0.015625f,  0.016204f),
        glm::vec2(-0.050781f,  0.564815f),
        glm::vec2(-0.143229f,  0.016204f),
        glm::vec2(-0.227865f,  0.099537f)
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
    std::vector<unsigned int> indices(vertices.size() / 6);
    for (unsigned int i = 0; i < indices.size(); ++i)
        indices[i] = i;

    renderer = std::make_unique<Renderer>("resource/vertex.glsl", "resource/fragment.glsl");
    auto mesh = std::make_shared<Mesh>(vertices, indices);
    renderer->addMesh(mesh);
    camera = std::make_unique<Camera>(width, height);
}