#include "ui/ImGuiManager.h"
#include <dependencies/imgui/imgui.h>
#include <dependencies/imgui/imgui_internal.h>
#include <dependencies/imgui/imgui_impl_glfw.h>
#include <dependencies/imgui/imgui_impl_opengl3.h>


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
    ImGui::SliderInt("Player Type", reinterpret_cast<int*>(&currentPlayerSelection), 0, 2);

    ImGui::Text("Sfondo");
    ImGui::SliderInt("Background", reinterpret_cast<int*>(&currentBackgroundSelection), 0, 3);

    ImGui::End();
}

