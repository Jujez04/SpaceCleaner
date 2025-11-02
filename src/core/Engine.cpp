#include "core/Engine.h"

#include <memory>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdexcept>

#include "core/InputManager.h"
#include "core/Window.h"
#include "graphics/Vertex.h"
#include "graphics/Shader.h"
#include "graphics/Camera.h"
#include "graphics/Mesh.h"
#include "graphics/Renderer.h"
#include "game/SpaceCleaner.h"
#include "utilities/Utilities.h"
#include "utilities/Timer.h"
#include "scene/Scene.h"
#include "math/HermiteMesh.h"
#include "graphics/ShaderManager.h"
#include "graphics/MeshManager.h"
#include "ui/ImGuiManager.h"

Engine::Engine() {}
Engine::~Engine() {}

void Engine::processInput() {
    if (currentState == GameState::GAME_OVER) {
        if (InputManager::isKeyPressed(GLFW_KEY_R)) {
            resetGame();
        }
        return;
    }

    if (InputManager::isKeyPressed(GLFW_KEY_W))
        player->transform.translate({ 0.0f, 0.001f });

    if (InputManager::isKeyPressed(GLFW_KEY_S))
        player->transform.translate({ 0.0f, -0.001f });

    if (InputManager::isKeyPressed(GLFW_KEY_A))
        player->transform.translate({ -0.001f, 0.0f });

    if (InputManager::isKeyPressed(GLFW_KEY_D))
        player->transform.translate({ 0.001f, 0.0f });

    if (InputManager::isKeyPressed(GLFW_KEY_Q))
        player->transform.rotate(0.006f);

    if (InputManager::isKeyPressed(GLFW_KEY_E))
        player->transform.rotate(-0.006f);

    if (InputManager::isKeyPressed(GLFW_KEY_SPACE)) {
        if (timeSinceLastShot >= fireCooldown) {
            timeSinceLastShot = 0.0f;

            glm::vec2 shipPos = player->transform.getPosition();
            float angle = player->transform.getRotation();

            glm::vec2 dir = glm::vec2(-sin(angle), cos(angle));

            float speed = 1.2f;
            glm::vec2 vel = dir * speed;
            float lifetime = 3.0f;

            if (scene)
                scene->spawnProjectile(shipPos, vel, lifetime,
                    this->projectileMeshId,
                    this->defaultShaderId,
                    glm::vec4(1.0f, 0.8f, 0.2f, 1.0f));
        }
    }

    if (InputManager::isKeyPressed(GLFW_KEY_F1)) {
        imguiVisible = !imguiVisible;
    }
}

void Engine::update(float delta) {
    if (currentState == GameState::GAME_OVER) {
        return;
    }
    timeSinceLastShot += delta;
    if (scene) {
        scene->update(delta);
        scene->checkCollisions();
        //Game over
        if (player->getHealth() == 0) {
            currentState = GameState::GAME_OVER;
        }
        scene->updateSpawning(delta,
            this->asteroidMeshId,
            this->cometMeshId,
            this->defaultShaderId);
    }
}

void Engine::rendering() {
    renderer->clear();
    float orthoWidth = camera->getWidth() / camera->getHeight(); // aspectRatio
    glm::mat4 bgModel = glm::scale(glm::mat4(1.0f), glm::vec3(orthoWidth, 1.0f, 1.0f));
    glm::mat4 viewIdentity = glm::mat4(1.0f);
    
    std::shared_ptr<Shader> backgroundShader = ShaderManager::get(this->backgroundShaderId);

    if (backgroundShader) {
        renderer->setCamera(viewIdentity, camera->getProjectionMatrix());
        backgroundShader->bind();
        backgroundShader->setUniformVec2("uResolution",
            glm::vec2(window->getWidth(), window->getHeight()));

        backgroundShader->setUniform1f("uTime", Timer::getTotalTime());

        backgroundShader->setUniformMat4("view", viewIdentity);
        backgroundShader->setUniformMat4("projection", camera->getProjectionMatrix());
        backgroundShader->setUniformMat4("model", bgModel);

        backgroundShader->setUniformVec4("uColor", glm::vec4(0.05f, 0.05f, 0.1f, 1.0f));
        auto mesh = MeshManager::getById(this->backgroundMeshId);
        if (mesh) {
            mesh->draw(*backgroundShader, GL_TRIANGLES);
        }

        backgroundShader->unbind();
    }
    float orthoHeight = 1.0f;

    renderer->setCamera(viewIdentity, camera->getProjectionMatrix());

    if (scene)
        scene->render(*renderer, GL_TRIANGLES);
    if (player) {
        float worldLeft = -(camera->getWidth() / camera->getHeight());
        renderer->drawEntityByInfo(*player, GL_TRIANGLES);
        glm::vec2 startPos = glm::vec2(worldLeft + 0.05f, 1.0f - 0.05f);
        float spacing = 0.08f;
        float heartScale = 0.1f;
        int currentHealth = player->getHealth();
        int maxHealth = player->getMaxHealth();
        for (int i = 0; i < maxHealth; ++i) {
            glm::vec4 color = (i < currentHealth) ?
                glm::vec4(1.0f, 0.0f, 0.4f, 1.0f) :
                glm::vec4(0.3f, 0.3f, 0.3f, 0.5f);
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(startPos.x + i * spacing, startPos.y, 0.0f));

            model = glm::scale(model, glm::vec3(heartScale));

            renderer->drawMesh(
                this->heartMeshId,
                this->defaultShaderId,
                color,
                model,
                GL_TRIANGLES
            );
        }
        
    }
    if (imguiVisible) {
        imguiManager->beginFrame();
        imguiManager->drawEditorWindow(this);
        imguiManager->endFrame();
    }
    window->updateWindow();

    
}

void Engine::resetGame() {
    currentState = PLAYING;
    player->resetHealth();
}


void Engine::gameLoop() {
    lastFrameTime = glfwGetTime();
    while (window->windowIsOpen()) {
        Timer::update();
        float delta = Timer::getDeltaTime();
        window->pollEvents();
        processInput();
        update(delta);
        rendering();
    }
}

void Engine::init() {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    int width = 1000;
    int height = 800;
    window = std::make_unique<Window>(width, height);

    camera = std::make_unique<Camera>(width, height);
    renderer = std::make_unique<Renderer>();

    // Callback per aggiornare la proiezione quando la finestra cambia dimensione
    window->setResizeCallback([this](int newWidth, int newHeight) {
        if (camera) {
            camera->setProjection(newWidth, newHeight);
        }
        });

    InputManager::init(window->getWindowReference());

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    scene = std::make_unique<Scene>();

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
    std::string vertexCode = readFile("resources/vertex.glsl");
    std::string fragmentCode = readFile("resources/fragment.glsl");
    unsigned int defaultShaderId = ShaderManager::load("DefaultShader", vertexCode, fragmentCode);
    player = std::make_shared<SpaceCleaner>("SpaceCleaner");
    unsigned int bodyMeshId = HermiteMesh::baseHermiteToMesh(player->getName(), controlPoints, 60);

    SubMeshRenderInfo bodyLayer(
        bodyMeshId,                                     // Mesh ID
        defaultShaderId,                                // Shader ID
        glm::vec4(0.0f, 0.4f, 0.8f, 1.0f)               // Colore azzurro solido
    );
    // La trasformazione locale è l'identità
    bodyLayer.localTransform = glm::scale(glm::mat4(1.0f), glm::vec3(0.3f, 0.3f, 1.0f));

    player->addMeshLayer(bodyLayer);

    std::vector<glm::vec2> circlePoints = {
        { 0.0f,  0.3f },
        { 0.212f,  0.212f },
        { 0.3f,  0.0f },
        { 0.212f, -0.212f },
        { 0.0f, -0.3f },
        { -0.212f, -0.212f },
        { -0.3f,  0.0f },
        { -0.212f,  0.212f },
        { 0.0f,  0.3f }
    };
    unsigned int circleMeshId = HermiteMesh::baseHermiteToMesh(player->getName(), circlePoints, 60);
    SubMeshRenderInfo circleLayer(
        circleMeshId,                  // Mesh ID generato
        defaultShaderId,               // Shader ID già caricato
        glm::vec4(0.8f, 0.8f, 0.0f, 1.0f)  // Colore giallo pieno
    );
    circleLayer.localTransform =
        glm::translate(glm::mat4(1.0f), glm::vec3(-0.0155f, -0.025f, 0.001f)) *  // piccolo offset
        glm::scale(glm::mat4(1.0f), glm::vec3(0.25f, 0.25f, 1.0f) * glm::vec3(0.3f, 0.3f, 1.0f));        // scala metà del corpo

    player->addMeshLayer(circleLayer);
    scene->addEntity(player);
    std::vector<glm::vec2> asteroidPoints = {
        glm::vec2(0.2f, 0.4f),
        glm::vec2(0.6f, 0.2f),
        glm::vec2(0.3f, -0.1f),
        glm::vec2(0.5f, -0.4f),
        glm::vec2(-0.2f, -0.5f),
        glm::vec2(-0.6f, -0.1f),
        glm::vec2(-0.4f, 0.3f)
    };
    asteroidMeshId = HermiteMesh::baseHermiteToMesh("AsteroidShape", asteroidPoints, 40);

    std::vector<glm::vec2> cometPoints = {
        glm::vec2(0.0f, 0.6f),
        glm::vec2(0.1f, 0.3f),
        glm::vec2(0.15f, 0.0f),
        glm::vec2(0.0f, -0.1f),
        glm::vec2(-0.15f, 0.0f),
        glm::vec2(-0.1f, 0.3f)
    };
    cometMeshId = HermiteMesh::baseHermiteToMesh("CometShape", cometPoints, 30);
    std::vector<glm::vec2> projectilePoints = {
        glm::vec2(0.0f, 0.05f),
        glm::vec2(0.015f, 0.0f),
        glm::vec2(0.0f, -0.05f),
        glm::vec2(-0.015f, 0.0f) 
    };
    unsigned int projectileMeshId = HermiteMesh::baseHermiteToMesh("ProjectileShape", projectilePoints, 10);
    this->projectileMeshId = projectileMeshId;
    this->defaultShaderId = defaultShaderId;
    std::vector<glm::vec2> heartPoints = {
        glm::vec2(0.003f, 0.16f),
        glm::vec2(0.124f, 0.301f),
        glm::vec2(0.177f, 0.178f),
        glm::vec2(0.186f, 0.012f),
        glm::vec2(0.0013f, -0.42f),
        glm::vec2(-0.186f, 0.012f),
        glm::vec2(-0.177f, 0.178f),
        glm::vec2(-0.124f, 0.301f),
        glm::vec2(0.003f, 0.16f)
    };
    heartMeshId = HermiteMesh::baseHermiteToMesh("HeartShape", heartPoints, 30);
    this->heartMeshId = heartMeshId;
    imguiManager = std::make_unique<ImGuiManager>(window->getWindowReference());
    std::vector<glm::vec2> quadPoints = {
    glm::vec2(-1.0f, 1.0f),  // Top Left
    glm::vec2(1.0f, 1.0f),   // Top Right
    glm::vec2(1.0f, -1.0f),  // Bottom Right
    glm::vec2(-1.0f, -1.0f)  // Bottom Left
    };
    unsigned int backgroundMeshId = HermiteMesh::baseHermiteToMesh("BackgroundQuad", quadPoints, 4);
    this->backgroundMeshId = backgroundMeshId;
    std::string bgVertexCode = readFile("resources/background_vertex.glsl");
    std::string bgFragmentCode = readFile("resources/fragmentSaturn.glsl");
    unsigned int backgroundShaderId = ShaderManager::load("BackgroundShader", bgVertexCode, bgFragmentCode);
    this->backgroundShaderId = backgroundShaderId;
}