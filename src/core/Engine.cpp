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
#include "game/Collision.h"
#include "math/CatmullRom.h"
#include "core/BackGround.h"
#include "core/PlayerConfig.h"


Engine::Engine() {}
Engine::~Engine() {}

void Engine::applyPlayerConfig(unsigned int configIndex) {
    if (!player || configIndex >= playerConfigs.size()) {
        return;
    }

    const auto& config = playerConfigs[configIndex];

    // 1. Svuota tutte le mesh esistenti dal player
    player->clearMeshLayers();

    // 2. Aggiungi i nuovi layer basati sulla configurazione

    // Layer Base
    SubMeshRenderInfo baseLayer(config.baseMeshId, this->defaultShaderId, config.base.color);
    baseLayer.localTransform = config.base.localTransform;
    player->addMeshLayer(baseLayer);

    // Layer Livrea
    SubMeshRenderInfo liveryLayer(config.liveryMeshId, this->defaultShaderId, config.livery.color);
    liveryLayer.localTransform = config.livery.localTransform;
    player->addMeshLayer(liveryLayer);

    // Layer Cockpit
    SubMeshRenderInfo cockpitLayer(config.cockpitMeshId, this->defaultShaderId, config.cockpit.color);
    cockpitLayer.localTransform = config.cockpit.localTransform;
    player->addMeshLayer(cockpitLayer);

    // Aggiorna la selezione corrente in ImGui
    imguiManager->currentPlayerSelection = configIndex;
}

void Engine::processInput() {
    if (currentState == GameState::GAME_OVER) {
        if (InputManager::isKeyPressed(GLFW_KEY_R)) {
            resetGame();
        }
        return;
    }

    if (InputManager::isKeyPressed(GLFW_KEY_W))
        player->transform.translate({ 0.0f, 0.002f });

    if (InputManager::isKeyPressed(GLFW_KEY_S))
        player->transform.translate({ 0.0f, -0.002f });

    if (InputManager::isKeyPressed(GLFW_KEY_A))
        player->transform.translate({ -0.002f, 0.0f });

    if (InputManager::isKeyPressed(GLFW_KEY_D))
        player->transform.translate({ 0.002f, 0.0f });

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
    if (imguiManager->currentPlayerSelection < playerConfigs.size()) {
        static unsigned int lastPlayerSelection = imguiManager->currentPlayerSelection;
        if (imguiManager->currentPlayerSelection != lastPlayerSelection) {
            applyPlayerConfig(imguiManager->currentPlayerSelection);
            lastPlayerSelection = imguiManager->currentPlayerSelection;
        }
    }
    timeSinceLastShot += delta;
    if (scene) {
        scene->update(delta);
        scene->checkCollisions();
        scoreManager.adjustScore(scene->getCollisions() * 100);
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
    
    if (imguiManager->currentBackgroundSelection < backgroundConfigs.size()) {
        unsigned int selectedId = backgroundConfigs[imguiManager->currentBackgroundSelection].shaderId;

        // 2. Aggiorna l'ID dello shader di background se la selezione è cambiata
        if (selectedId != this->backgroundShaderId) {
            this->backgroundShaderId = selectedId;
        }
    }

    // Usa l'ID aggiornato per recuperare lo shared_ptr<Shader> tramite ShaderManager::get(ID)
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
    if (imguiManager->boundingBoxMode) {
        if (scene) {
            for (const auto& entity : scene->getEntities()) {
                AABB box = entity->getAABB();
                renderer->drawBoundingBox(box, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
                    viewIdentity, camera->getProjectionMatrix());
            }
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
    scoreManager.reset();
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
    imguiManager = std::make_unique<ImGuiManager>(window->getWindowReference());
    scene = std::make_unique<Scene>();
    /*
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
    */
    std::string vertexCode = readFile("resources/vertex.glsl");
    std::string fragmentCode = readFile("resources/fragment.glsl");
    unsigned int defaultShaderId = ShaderManager::load("DefaultShader", vertexCode, fragmentCode);
    player = std::make_shared<SpaceCleaner>("SpaceCleaner", this);
    int segments = 80;

    /*
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
    */
    PlayerConfig model1 = {
        "Star Cruiser A",
        // Base
        { "PlayerBase", glm::vec4(0.5f, 0.5f, 0.5f, 1.0f), glm::scale(glm::mat4(1.0f), glm::vec3(0.3f)) },
        // Livrea (gialla)
        { "PlayerLivery", glm::vec4(0.9f, 0.9f, 0.0f, 1.0f), glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(0.31f)), glm::vec3(-0.01f, 0.02f, 0.1f)) },
        // Cockpit
        { "PlayerCockpit", glm::vec4(0.1f, 0.1f, 0.1f, 1.0f), glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(0.3f)), glm::vec3(-0.04f, 0.1f, 0.2f)) }
    };

    // Definizione del Modello 2 (Esempio, usando i file base ma con trasformazioni/colori diversi)
    PlayerConfig model2 = {
        "Star Cruiser B (Red)",
        // Base (più grande e scuro)
        { "PlayerBase", glm::vec4(0.3f, 0.3f, 0.3f, 1.0f), glm::scale(glm::mat4(1.0f), glm::vec3(0.4f)) },
        // Livrea (rossa)
        { "PlayerLivery", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(0.41f)), glm::vec3(-0.01f, 0.02f, 0.1f)) },
        // Cockpit (blu)
        { "PlayerCockpit", glm::vec4(0.2f, 0.2f, 0.8f, 1.0f), glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(0.4f)), glm::vec3(-0.04f, 0.1f, 0.2f)) }
    };

    playerConfigs.push_back(model1);
    playerConfigs.push_back(model2); // Aggiungi il secondo modello

    // 1. Carica le mesh una sola volta
    unsigned int baseId = HermiteMesh::catmullRomToMesh("PlayerBase", "resources/BaseSWship.txt", 40);
    unsigned int liveryId = HermiteMesh::catmullRomToMesh("PlayerLivery", "resources/LiverySWship.txt", 40);
    unsigned int cockpitId = HermiteMesh::catmullRomToMesh("PlayerCockpit", "resources/CockpitSWship.txt", 40);

    // 2. Assegna gli ID delle mesh caricate a *tutte* le configurazioni
    for (auto& config : playerConfigs) {
        config.baseMeshId = baseId;
        config.liveryMeshId = liveryId;
        config.cockpitMeshId = cockpitId;
    }

    // 3. Applica la configurazione iniziale (Modello 1, indice 0)
    applyPlayerConfig(0);
    /*
    unsigned int baseId = HermiteMesh::catmullRomToMesh("PlayerBase", "resources/BaseSWship.txt", 40);
    unsigned int liveryId = HermiteMesh::catmullRomToMesh("PlayerLivery", "resources/LiverySWship.txt", 40);
    unsigned int cockpitId = HermiteMesh::catmullRomToMesh("PlayerCockpit", "resources/CockpitSWship.txt", 40);

    SubMeshRenderInfo baseLayer(baseId, defaultShaderId, glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
    SubMeshRenderInfo liveryLayer(liveryId, defaultShaderId, glm::vec4(0.9f, 0.9f, 0.0f, 1.0f));
    SubMeshRenderInfo cockpitLayer(cockpitId, defaultShaderId, glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));

    baseLayer.localTransform = glm::scale(glm::mat4(1.0f), glm::vec3(0.3f));
    liveryLayer.localTransform = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(0.31f)), glm::vec3(-0.01f, 0.02f, 0.1f));
    cockpitLayer.localTransform = glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(0.3f)), glm::vec3(-0.04f, 0.1f, 0.2f));

    player->addMeshLayer(baseLayer);
    player->addMeshLayer(liveryLayer);
    player->addMeshLayer(cockpitLayer);
    */
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
    
    std::vector<glm::vec2> quadPoints = {
        glm::vec2(-1.0f, 1.0f),  // Top Left
        glm::vec2(1.0f, 1.0f),   // Top Right
        glm::vec2(1.0f, -1.0f),  // Bottom Right
        glm::vec2(-1.0f, -1.0f)  // Bottom Left
    };
    unsigned int backgroundMeshId = HermiteMesh::baseHermiteToMesh("BackgroundQuad", quadPoints, 4);
    this->backgroundMeshId = backgroundMeshId;
    std::string bgVertexCode = readFile("resources/background_vertex.glsl");

    // 2. Definisci le configurazioni di background
    backgroundConfigs.push_back({
        "Default Blue",
        "resources/fragmentDefaultBlue.glsl", // Assumi che esista
        0
        });
    backgroundConfigs.push_back({
        "Stellar",
        "resources/fragmentSaturn.glsl", // Il tuo shader esistente
        0
        });
    backgroundConfigs.push_back({
        "Star Field",
        "resources/fragmentStarField.glsl", // Assumi che esista
        0
        });

    // 3. Pre-carica tutti gli shader e assegna gli ID
    for (auto& config : backgroundConfigs) {
        std::string fragmentCode = readFile(config.fragmentShaderPath);
        config.shaderId = ShaderManager::load(config.name, bgVertexCode, fragmentCode);
    }

    // 4. Imposta lo shader iniziale sul primo della lista
    if (!backgroundConfigs.empty()) {
        this->backgroundShaderId = backgroundConfigs[0].shaderId;
        imguiManager->currentBackgroundSelection = 0; // Inizializza l'indice ImGui
    }
    this->backgroundShaderId = backgroundShaderId;
}