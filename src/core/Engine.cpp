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
#include "game/SpaceCleaner.h"
#include "utilities/Utilities.h"

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
    if (player)
        renderer->drawEntity(*player, GL_TRIANGLES);
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
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
    player = std::make_unique<SpaceCleaner>("SpaceCleaner");
    player->generateHermiteMesh(controlPoints, 60);
    player->getMeshComp().getMeshId();
    player->getColorComp().setColor(glm::vec4(0.0f, 0.4f, 0.8f, 1.0f)); 
    std::string vertexCode = readFile("resources/vertex.glsl");
    std::string fragmentCode = readFile("resources/fragment.glsl");
    player->color.setColor(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
    renderer = std::make_unique<Renderer>(vertexCode, fragmentCode);

    
    camera = std::make_unique<Camera>(width, height);
}