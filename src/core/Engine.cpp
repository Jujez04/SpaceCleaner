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
#include "core/ResourceLoader.h"

// Costruttore e distruttore
Engine::Engine() {}
Engine::~Engine() {}


/**
 * Applica una configurazione visiva al giocatore
 * @param configIndex Indice della configurazione da applicare (0-based)
 *
 * Ogni configurazione contiene 3 layer visivi:
 * - Base: struttura principale della nave
 * - Livery: decorazione/livrea
 * - Cockpit: cabina di pilotaggio
 */
void Engine::applyPlayerConfig(unsigned int configIndex) {
    // Verifica validità: player esistente e indice valido
    if (!player || configIndex >= playerConfigs.size()) {
        return;
    }

    const auto& config = playerConfigs[configIndex];

    // Rimuove tutti i layer visivi precedenti
    player->clearMeshLayers();

    // --- LAYER 1: BASE ---
    // Struttura principale della navicella
    SubMeshRenderInfo baseLayer(config.baseMeshId, this->defaultShaderId, config.base.color);
    baseLayer.localTransform = config.base.localTransform;
    player->addMeshLayer(baseLayer);

    // --- LAYER 2: LIVERY ---
    // Decorazioni e pattern della nave
    SubMeshRenderInfo liveryLayer(config.liveryMeshId, this->defaultShaderId, config.livery.color);
    liveryLayer.localTransform = config.livery.localTransform;
    player->addMeshLayer(liveryLayer);

    // --- LAYER 3: COCKPIT ---
    // Cabina di pilotaggio
    SubMeshRenderInfo cockpitLayer(config.cockpitMeshId, this->defaultShaderId, config.cockpit.color);
    cockpitLayer.localTransform = config.cockpit.localTransform;
    player->addMeshLayer(cockpitLayer);

    // Sincronizza la selezione con l'interfaccia ImGui
    imguiManager->currentPlayerSelection = configIndex;
}

/**
 * Processa tutti gli input dell'utente con logica gerarchica basata sullo stato
 *
 * Priorità:
 * 1. Tasti di sistema (ESC, F1)
 * 2. Tasti di stato (R per reset, SPACE per start)
 * 3. Controlli di gioco (movimento, sparo)
 */
void Engine::processInput() {

    // --- ESC: Pausa/Riprendi/Esci ---
    if (InputManager::isKeyPressed(GLFW_KEY_ESCAPE) && timeSinceLastEsc >= escCooldown) {
        timeSinceLastEsc = 0.0f;

        if (currentState == GameState::PLAYING) {
            // Durante il gioco: metti in pausa
            currentState = GameState::PAUSED;
        }
        else if (currentState == GameState::PAUSED) {
            // In pausa: riprendi il gioco
            currentState = GameState::PLAYING;
        }
        else if (currentState == GameState::START) {
            // Nel menu iniziale: chiudi il gioco
            window->setShouldClose(true);
        }
        // NOTA: in GAME_OVER, ESC non fa nulla (usa R per resettare)
        return;
    }

    // --- F1: Toggle Debug/Editor UI ---
    if (InputManager::isKeyPressed(GLFW_KEY_F1) && timeSinceLastF1 >= f1Cooldown) {
        imguiVisible = !imguiVisible;
        timeSinceLastF1 = 0.0f;
    }

    if (currentState == GameState::GAME_OVER) {
        // R: Resetta e riavvia il gioco
        if (InputManager::isKeyPressed(GLFW_KEY_R)) {
            resetGame();
        }
        // Blocca tutti gli altri input durante GAME_OVER
        return;
    }

    if (currentState == GameState::START || currentState == GameState::PAUSED) {
        // SPACE nel menu START: avvia il gioco
        if (currentState == GameState::START && InputManager::isKeyPressed(GLFW_KEY_SPACE)) {
            currentState = GameState::PLAYING;
            resetGame(); // Resetta salute e punteggio
            return;
        }
        // Blocca tutti gli input di gioco in START e PAUSED
        return;
    }

	// CONTROLLI DI GIOCO (solo in PLAYING)

    // --- MOVIMENTO TRASLAZIONALE ---
    if (InputManager::isKeyPressed(GLFW_KEY_W))
        player->transform.translate({ 0.0f, 0.02f });   // Su

    if (InputManager::isKeyPressed(GLFW_KEY_S))
        player->transform.translate({ 0.0f, -0.02f });  // Giù

    if (InputManager::isKeyPressed(GLFW_KEY_A))
        player->transform.translate({ -0.02f, 0.0f });  // Sinistra

    if (InputManager::isKeyPressed(GLFW_KEY_D))
        player->transform.translate({ 0.02f, 0.0f });   // Destra

    // --- MOVIMENTO ROTAZIONALE ---
    if (InputManager::isKeyPressed(GLFW_KEY_Q))
        player->transform.rotate(0.06f);   // Ruota in senso antiorario

    if (InputManager::isKeyPressed(GLFW_KEY_E))
        player->transform.rotate(-0.06f);  // Ruota in senso orario

    // --- SPARO PROIETTILI ---
    if (InputManager::isKeyPressed(GLFW_KEY_SPACE)) {
        // Cooldown per evitare sparo continuo
        if (timeSinceLastShot >= fireCooldown) {
            timeSinceLastShot = 0.0f;

            // Calcola posizione e direzione del proiettile
            glm::vec2 shipPos = player->transform.getPosition();
            float angle = player->transform.getRotation();

            // Direzione basata sull'angolo di rotazione della nave
            glm::vec2 dir = glm::vec2(-sin(angle), cos(angle));

            // Parametri del proiettile
            float speed = 1.2f;
            glm::vec2 vel = dir * speed;
            float lifetime = 3.0f;  // Secondi prima della distruzione

            // Spawna il proiettile nella scena
            if (scene)
                scene->spawnProjectile(shipPos, vel, lifetime,
                    this->projectileMeshId,
                    this->defaultShaderId,
                    glm::vec4(1.0f, 0.8f, 0.2f, 1.0f));  // Colore giallo-arancione
        }
    }
}

/**
 * Aggiorna la logica di gioco ogni frame
 * @param delta Tempo trascorso dall'ultimo frame (in secondi)
 */
void Engine::update(float delta) {
    // Aggiorna i timer di cooldown
    timeSinceLastF1 += delta;
    timeSinceLastEsc += delta;

    // Controlla se l'utente ha cambiato nave nell'editor
    if (imguiManager->currentPlayerSelection < playerConfigs.size()) {
        static unsigned int lastPlayerSelection = imguiManager->currentPlayerSelection;
        if (imguiManager->currentPlayerSelection != lastPlayerSelection) {
            applyPlayerConfig(imguiManager->currentPlayerSelection);
            lastPlayerSelection = imguiManager->currentPlayerSelection;
        }
    }

    // Non aggiornare la logica se non in PLAYING
    if (currentState == GameState::GAME_OVER ||
        currentState == GameState::PAUSED ||
        currentState == GameState::START) {
        return;
    }

	// Aggiornamento timer sparo
    timeSinceLastShot += delta;

	// Aggiornamento scena di gioco
    if (scene) {
        // Aggiorna fisica ed entità
        scene->update(delta);

        // Rileva e gestisce collisioni
        scene->checkCollisions();

        // Incrementa punteggio per ogni collisione (100 punti)
        scoreManager.adjustScore(scene->getCollisions() * 100);

        // Se ha 0 cuori -> Game Over
        if (player->getHealth() == 0) {
            currentState = GameState::GAME_OVER;
        }

        // Spawna nuovi nemici (asteroidi, comete)
        scene->updateSpawning(delta,
            this->asteroidMeshId,
            this->cometMeshId,
            this->defaultShaderId);
    }
}

/**
 * Renderizza un frame completo del gioco
 *
 * Ordine di rendering:
 * 1. Background animato
 * 2. Entità di gioco (nemici, proiettili)
 * 3. Player
 * 4. UI (cuori, bounding box debug)
 * 5. ImGui (editor, menu)
 */
void Engine::rendering() {
    if (!window) {
        return;
    }

    // Pulisce il buffer dello schermo
    renderer->clear();

	// BACKGROUND ANIMATO

    // Calcola l'aspect ratio per lo sfondo
    float orthoWidth = camera->getWidth() / camera->getHeight();
    glm::mat4 bgModel = glm::scale(glm::mat4(1.0f), glm::vec3(orthoWidth, 1.0f, 1.0f));
    glm::mat4 viewIdentity = glm::mat4(1.0f);

    // Aggiorna lo shader di background se cambiato dall'utente
    if (imguiManager->currentBackgroundSelection < backgroundConfigs.size()) {
        unsigned int selectedId = backgroundConfigs[imguiManager->currentBackgroundSelection].shaderId;

        if (selectedId != this->backgroundShaderId) {
            this->backgroundShaderId = selectedId;
        }
    }

    // Recupera e configura lo shader del background
    std::shared_ptr<Shader> backgroundShader = ShaderManager::get(this->backgroundShaderId);
    if (backgroundShader) {
        renderer->setCamera(viewIdentity, camera->getProjectionMatrix());
        backgroundShader->bind();

        // Passa parametri per animazione shader
        backgroundShader->setUniformVec2("uResolution",
            glm::vec2(window->getWidth(), window->getHeight()));
        backgroundShader->setUniform1f("uTime", Timer::getTotalTime());

        // Matrici di trasformazione
        backgroundShader->setUniformMat4("view", viewIdentity);
        backgroundShader->setUniformMat4("projection", camera->getProjectionMatrix());
        backgroundShader->setUniformMat4("model", bgModel);

        // Colore base del background
        backgroundShader->setUniformVec4("uColor", glm::vec4(0.05f, 0.05f, 0.1f, 1.0f));

        // Disegna la mesh del background
        auto mesh = MeshManager::getById(this->backgroundMeshId);
        if (mesh) {
            mesh->draw(*backgroundShader, GL_TRIANGLES);
        }

        backgroundShader->unbind();
    }

    // ENTITÀ DI GIOCO

    renderer->setCamera(viewIdentity, camera->getProjectionMatrix());

    // Renderizza tutti i nemici e proiettili
    if (scene)
        scene->render(*renderer, GL_TRIANGLES);


    // PLAYER e CUORI

    if (player) {
        // Disegna la navicella del giocatore
        float worldLeft = -(camera->getWidth() / camera->getHeight());
        renderer->drawEntityByInfo(*player, GL_TRIANGLES);

		// Disegna i cuori della salute in alto a sinistra
        glm::vec2 startPos = glm::vec2(worldLeft + 0.05f, 1.0f - 0.05f);  // Angolo in alto a sinistra
        float spacing = 0.08f;      // Distanza tra cuori
        float heartScale = 0.1f;    // Dimensione cuori

        int currentHealth = player->getHealth();
        int maxHealth = player->getMaxHealth();

        // Disegna i cuori (pieni = salute, vuoti = salute persa)
        for (int i = 0; i < maxHealth; ++i) {
            // Colore: rosso pieno se attivo, grigio trasparente se vuoto
            glm::vec4 color = (i < currentHealth) ?
                glm::vec4(1.0f, 0.0f, 0.4f, 1.0f) :      // Rosa acceso
                glm::vec4(0.3f, 0.3f, 0.3f, 0.5f);       // Grigio trasparente

            // Posiziona e scala il cuore
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(startPos.x + i * spacing, startPos.y, 0.0f));
            model = glm::scale(model, glm::vec3(heartScale));

            // Renderizza il cuore
            renderer->drawMesh(
                this->heartMeshId,
                this->defaultShaderId,
                color,
                model,
                GL_TRIANGLES
            );
        }
    }

    // DEBUG: BOUNDING BOX

    // Mostra i bounding box per debug collisioni
    if (imguiManager->boundingBoxMode) {
        if (scene) {
            for (const auto& entity : scene->getEntities()) {
                AABB box = entity->getAABB();
                renderer->drawBoundingBox(box, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f),
                    viewIdentity, camera->getProjectionMatrix());
            }
        }
    }

    // INTERFACCIA IMGUI

    // Mostra ImGui se visibile o se non in gioco
    if (imguiVisible || currentState != GameState::PLAYING) {
        imguiManager->beginFrame();

        // Editor/Debug UI
        if (imguiVisible) {
            imguiManager->drawEditorWindow(this);
        }

        // Menu contestuali basati sullo stato
        if (currentState == GameState::START) {
            imguiManager->drawStartMenu(this);
        }
        else if (currentState == GameState::PAUSED) {
            imguiManager->drawPauseMenu(this);
        }
        else if (currentState == GameState::GAME_OVER) {
            imguiManager->drawGameOverMenu(this);
        }

        imguiManager->endFrame();
    }

    // Aggiorna il buffer della finestra (swap buffers)
    window->updateWindow();
}

/**
 * Resetta il gioco allo stato iniziale
 * - Ripristina salute del player
 * - Azzera punteggio
 * - Torna allo stato PLAYING
 */
void Engine::resetGame() {
    currentState = PLAYING;
    player->resetHealth();
    scoreManager.reset();
}

/**
 * Loop principale del gioco (60 FPS)
 * La funzione è un'implementazione del game loop pattern.
 * 
 * Pattern:
 * 1. Aggiorna timer e calcola deltaTime
 * 2. Processa eventi finestra
 * 3. Gestisci input
 * 4. Aggiorna logica
 * 5. Renderizza
 */
void Engine::gameLoop() {
    Timer::init(); // Inizializza il sistema di timing

    while (window->windowIsOpen()) {
        Timer::update();                  // Lock a 60 FPS
        float delta = Timer::getDeltaTime();

        window->pollEvents();             // Eventi GLFW (resize, close, etc.)
        processInput();                   // Input utente
        update(delta);                    // Logica di gioco
        rendering();                      // Rendering
    }
}

// INIZIALIZZAZIONE

/**
 * Inizializza l'intero engine
 *
 * Setup:
 * 1. Contesto OpenGL/GLFW
 * 2. Finestra e camera
 * 3. Renderer e input
 * 4. ImGui
 * 5. Caricamento risorse (shader, mesh, configurazioni)
 * 6. Creazione player e scene
 */
void Engine::init() {
	// Configurazione GLFW per OpenGL 3.3 Core Profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	// Creazione finestra di gioco
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

	// Inizializza InputManager e l'API di ImGUI con il riferimento alla finestra GLFW
    InputManager::init(window->getWindowReference());
    imguiManager = std::make_unique<ImGuiManager>(window->getWindowReference());
    scene = std::make_unique<Scene>();

    // Abilita blending per trasparenza
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    
    // CARICAMENTO RISORSE

    // Usa ResourceLoader per caricare shader, mesh e configurazioni
    ResourceLoader resourceLoader;
    resourceLoader.loadAllResources(
        this->defaultShaderId,
        this->projectileMeshId,
        this->heartMeshId,
        this->backgroundMeshId,
        this->playerConfigs,
        this->backgroundConfigs
    );

	/**
    * CREAZIONE PLAYER E MESH PERSONALIZZATE
    * In questa sezione ho voluto far vedere un dettaglio implementativo.
    * Inizialmente le entità venivano create con mesh interne al codice.
	* Ho intenzionalmente mantenuto questa parte per mostrare il processo di creazione
    * e di evoluzione in termini di design della creazione di una mesh.
    */
    player = std::make_shared<SpaceCleaner>("SpaceCleaner", this);

    // --- ASTEROIDE ---
    // Forma irregolare con 7 punti di controllo
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

    // --- COMETA ---
    // Forma allungata con 6 punti di controllo
    std::vector<glm::vec2> cometPoints = {
        glm::vec2(0.0f, 0.6f),
        glm::vec2(0.1f, 0.3f),
        glm::vec2(0.15f, 0.0f),
        glm::vec2(0.0f, -0.1f),
        glm::vec2(-0.15f, 0.0f),
        glm::vec2(-0.1f, 0.3f)
    };
    cometMeshId = HermiteMesh::baseHermiteToMesh("CometShape", cometPoints, 30);


    // Imposta lo shader di background iniziale
    if (!backgroundConfigs.empty()) {
        this->backgroundShaderId = backgroundConfigs[0].shaderId;
        imguiManager->currentBackgroundSelection = 0;
    }

    // Aggiungi il player alla scena
    scene->addEntity(player);

    // Applica la configurazione iniziale del player (Modello 1, indice 0)
    applyPlayerConfig(0);
}