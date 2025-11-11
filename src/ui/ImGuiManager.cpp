#pragma once
#include "ui/ImGuiManager.h"

#include "core/Engine.h"
#include "core/BackGround.h"
#include "core/Window.h"

ImGuiManager::ImGuiManager(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.FontGlobalScale = 2.0f;

    ImGui::StyleColorsDark();

    // Inizializza i backend
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}


ImGuiManager::~ImGuiManager() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiManager::beginFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGuiManager::endFrame() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiManager::drawPauseMenu(Engine* engine) {
    // Posiziona la finestra al centro dello schermo
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f),
        ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(350, 180));

    ImGuiWindowFlags windowFlags =
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove; // Rimuove bordi e titolo

    if (ImGui::Begin("PauseMenu", nullptr, windowFlags)) {
        ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "  >> GIOCO IN PAUSA <<");
        ImGui::Separator();

        // Riprendi (ESC è gestito in processInput, ma offriamo il bottone)
        if (ImGui::Button("Riprendi", ImVec2(-1, 0))) {
            engine->setCurrentState(PLAYING);
        }

        // Ricomincia
        if (ImGui::Button("Ricomincia", ImVec2(-1, 0))) {
            engine->resetGame();
        }

        // Esci
        if (ImGui::Button("Esci dal Gioco", ImVec2(-1, 0))) {
            // Assumendo che Engine abbia un modo per chiudere la finestra, ad esempio tramite il suo puntatore window
            // Nota: Se Engine->window non è accessibile, questa parte andrebbe modificata.
            if (engine && engine->window) {
                engine->window->setShouldClose(true);
            }
        }
    }
    ImGui::End();
}

void ImGuiManager::drawGameOverMenu(Engine* engine) {
    // Posiziona la finestra al centro dello schermo
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f),
        ImGuiCond_Always, ImVec2(0.5f, 0.5f));

    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize;

    if (ImGui::Begin("GameOver", nullptr, windowFlags)) {
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "  >>> G A M E   O V E R <<<");
        ImGui::Separator();

        ImGui::Text("Punti totali: %d", engine->getScore());
        ImGui::Separator();

        if (ImGui::Button("Riprova (R)", ImVec2(-1, 0))) {
            engine->resetGame();
        }
        if (ImGui::Button("Esci", ImVec2(-1, 0))) {
            if (engine && engine->window) {
                engine->window->setShouldClose(true);
            }
        }
    }
    ImGui::End();
}

void ImGuiManager::drawStartMenu(Engine* engine) {
    // Posiziona la finestra al centro dello schermo
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f),
        ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(600, 200));

    ImGuiWindowFlags windowFlags =
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove;

    if (ImGui::Begin("StartMenu", nullptr, windowFlags)) {
        ImGui::TextColored(ImVec4(0.0f, 0.8f, 1.0f, 1.0f), "  >> SPACECLEANER <<");
        ImGui::Separator();
        ImGui::Spacing();

        ImGui::Text("Sgombra lo spazio dai detriti!");
        ImGui::Spacing();
        ImGui::Separator();

        if (ImGui::Button("Avvia Partita", ImVec2(-1, 0))) {
            // Avvia il gioco!
            engine->setCurrentState(PLAYING);
            engine->resetGame(); // Opzionale: assicurati che il gioco sia pulito
        }

        ImGui::Spacing();

        if (ImGui::Button("Esci", ImVec2(-1, 0))) {
            if (engine && engine->window) {
                engine->window->setShouldClose(true);
            }
        }
    }
    ImGui::End();
}


// Editor Window

void ImGuiManager::drawEditorWindow(Engine* engine) {
    ImGui::Begin("SpaceCleaner Editor");

    ImGui::Text("Controlli di rendering");
    if (ImGui::Checkbox("Wireframe Mode", &wireframeMode)) {
        if (wireframeMode)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    ImGui::Checkbox("Bounding Box Mode", &boundingBoxMode);

    ImGui::Separator();
    ImGui::Text("Scelta giocatore");
    const auto& playerConfigs = engine->playerConfigs;
    std::vector<const char*> playerItems;
    for (const auto& config : playerConfigs) {
        playerItems.push_back(config.name.c_str());
    }

    if (!playerItems.empty()) {
        if (ImGui::Combo("Player Model", (int*)&currentPlayerSelection, playerItems.data(), (int)playerItems.size())) {
            // La logica di applicazione è in Engine::update().
        }
    }
    else {
        ImGui::TextDisabled("Nessun modello di player configurato.");
    }
    const auto& bgConfigs = engine->backgroundConfigs;
    std::vector<const char*> bgItems;
    for (const auto& config : bgConfigs) {
        bgItems.push_back(config.name.c_str());
    }

    if (!bgItems.empty()) {
        if (ImGui::Combo("Shader Select", (int*)&currentBackgroundSelection, bgItems.data(), (int)bgItems.size())) {
            // Engine::rendering() userà l'indice aggiornato.
        }
    }
    else {
        ImGui::TextDisabled("Nessun background configurato.");
    }
    ImGui::Separator();
    ImGui::Text("Current Score: %d", engine->getScore());
    ImGui::Separator();
    ImGui::End();
}