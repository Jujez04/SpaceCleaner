#include "ui/ImGuiManager.h"
#include "core/Engine.h"
#include "core/BackGround.h"
#include "core/Window.h"

/**
 * @brief Costruttore: inizializza ImGui e i backend GLFW/OpenGL.
 */
ImGuiManager::ImGuiManager(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.FontGlobalScale = 2.0f; // ingrandisce font di default
    ImGui::StyleColorsDark();   // tema scuro

    // Inizializzazione backend
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

/**
 * @brief Distruttore: esegue il cleanup completo di ImGui.
 */
ImGuiManager::~ImGuiManager() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

/**
 * @brief Inizia un nuovo frame ImGui (preparazione rendering GUI).
 */
void ImGuiManager::beginFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

/**
 * @brief Termina il frame e invia i dati di rendering a OpenGL.
 */
void ImGuiManager::endFrame() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

/**
 * @brief Disegna il menu di pausa centrato nello schermo.
 *
 * Include pulsanti per riprendere, ricominciare o uscire dal gioco.
 */
void ImGuiManager::drawPauseMenu(Engine* engine) {
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f,
        ImGui::GetIO().DisplaySize.y * 0.5f),
        ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(350, 180));
    ImGuiWindowFlags windowFlags =
        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

    if (ImGui::Begin("PauseMenu", nullptr, windowFlags)) {
        ImGui::TextColored(ImVec4(1, 1, 0, 1), "  >> GIOCO IN PAUSA <<");
        ImGui::Separator();
        if (ImGui::Button("Riprendi", ImVec2(-1, 0))) engine->setCurrentState(PLAYING);
        if (ImGui::Button("Ricomincia", ImVec2(-1, 0))) engine->resetGame();
        if (ImGui::Button("Esci dal Gioco", ImVec2(-1, 0))) {
            if (engine && engine->window) engine->window->setShouldClose(true);
        }
    }
    ImGui::End();
}

/**
 * @brief Disegna il menu Game Over centrato nello schermo.
 *
 * Mostra il punteggio e pulsanti per riprovare o uscire.
 */
void ImGuiManager::drawGameOverMenu(Engine* engine) {
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f,
        ImGui::GetIO().DisplaySize.y * 0.5f),
        ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize;

    if (ImGui::Begin("GameOver", nullptr, windowFlags)) {
        ImGui::TextColored(ImVec4(1, 0, 0, 1), "  >>> G A M E   O V E R <<<");
        ImGui::Separator();
        ImGui::Text("Punti totali: %d", engine->getScore());
        ImGui::Separator();
        if (ImGui::Button("Riprova (R)", ImVec2(-1, 0))) engine->resetGame();
        if (ImGui::Button("Esci", ImVec2(-1, 0))) {
            if (engine && engine->window) engine->window->setShouldClose(true);
        }
    }
    ImGui::End();
}

/**
 * @brief Disegna il menu di Start centrato nello schermo.
 *
 * Include pulsanti per avviare il gioco o uscire e mostra informazioni base.
 */
void ImGuiManager::drawStartMenu(Engine* engine) {
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f,
        ImGui::GetIO().DisplaySize.y * 0.5f),
        ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(600, 310));
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

    if (ImGui::Begin("StartMenu", nullptr, windowFlags)) {
        ImGui::TextColored(ImVec4(0, 0.8f, 1, 1), "  >> SPACECLEANER <<");
        ImGui::Separator();
        ImGui::Text("WASD -> movimento");
        ImGui::Text("Spazio -> spara proiettili");
        ImGui::Text("F1 -> editor");
        ImGui::Text("Esc -> pausa");
        ImGui::Text("Spara agli ostacoli per ottenere");
		ImGui::Text("un punteggio migliore");
        ImGui::Separator();

        if (ImGui::Button("Avvia Partita", ImVec2(-1, 0))) {
            engine->setCurrentState(PLAYING);
            engine->resetGame();
        }

        if (ImGui::Button("Esci", ImVec2(-1, 0))) {
            if (engine && engine->window) engine->window->setShouldClose(true);
        }
    }
    ImGui::End();
}

/**
 * @brief Disegna la finestra editor per controlli runtime.
 *
 * Consente di modificare modalità wireframe, mostrare bounding box, selezionare player/shader
 * e visualizzare lo score corrente.
 */
void ImGuiManager::drawEditorWindow(Engine* engine) {
    ImGui::Begin("SpaceCleaner Editor");

    ImGui::Text("Controlli di rendering");
    if (ImGui::Checkbox("Wireframe Mode", &wireframeMode))
        glPolygonMode(GL_FRONT_AND_BACK, wireframeMode ? GL_LINE : GL_FILL);
    ImGui::Checkbox("Bounding Box Mode", &boundingBoxMode);

    ImGui::Separator();
    ImGui::Text("Scelta giocatore");
    std::vector<const char*> playerItems;
    for (const auto& config : engine->playerConfigs) playerItems.push_back(config.name.c_str());

    if (!playerItems.empty())
        ImGui::Combo("Player Model", (int*)&currentPlayerSelection, playerItems.data(), (int)playerItems.size());
    else
        ImGui::TextDisabled("Nessun modello di player configurato.");

    std::vector<const char*> bgItems;
    for (const auto& config : engine->backgroundConfigs) bgItems.push_back(config.name.c_str());

    if (!bgItems.empty())
        ImGui::Combo("Shader Select", (int*)&currentBackgroundSelection, bgItems.data(), (int)bgItems.size());
    else
        ImGui::TextDisabled("Nessun background configurato.");

    ImGui::Separator();
    ImGui::Text("Current Score: %d", engine->getScore());
    ImGui::Separator();
    ImGui::End();
}
