#include "ui/ImGuiManager.h"
#include <dependencies/imgui/imgui.h>
#include <dependencies/imgui/imgui_internal.h>
#include <dependencies/imgui/imgui_impl_glfw.h>
#include <dependencies/imgui/imgui_impl_opengl3.h>
#include "core/Engine.h"
#include "core/BackGround.h"

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
        // currentPlayerSelection è l'indice (unsigned int)
        if (ImGui::Combo("Player Model", (int*)&currentPlayerSelection, playerItems.data(), (int)playerItems.size())) {
            // L'indice 'currentPlayerSelection' è stato aggiornato.
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
        // currentBackgroundSelection è l'indice (unsigned int) in ImGuiManager.h
        if (ImGui::Combo("Shader Select", (int*)&currentBackgroundSelection, bgItems.data(), (int)bgItems.size())) {
            // L'indice è ora aggiornato. Engine::rendering() lo userà nel frame successivo.
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

