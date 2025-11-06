#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h> // Richiesto per l'inizializzazione GLFW di ImGui
#include <memory>
#include <functional>
#include <dependencies/imgui/imgui.h>
#include <dependencies/imgui/imgui_internal.h>
#include <dependencies/imgui/imgui_impl_glfw.h>
#include <dependencies/imgui/imgui_impl_opengl3.h>

class Engine;

class ImGuiManager {
public:
    // Il costruttore di default
    ImGuiManager() = default;
    // Il costruttore prende il puntatore alla finestra GLFW
    ImGuiManager(GLFWwindow* window);
    // Il distruttore gestirà il cleanup ImGui
    ~ImGuiManager();

    // Metodo chiamato all'inizio di ogni frame di rendering del motore
    void beginFrame();

    // Metodo chiamato alla fine di ogni frame di rendering del motore
    void endFrame();

    // Metodo per disegnare la finestra dell'editor
    void drawEditorWindow(Engine* engine);
    
    void drawPauseMenu(Engine* engine);

    void drawGameOverMenu(Engine* engine);

    void drawStartMenu(Engine* engine);
    // Variabili di stato che verranno manipolate dall'Engine
    bool wireframeMode = false;
    bool boundingBoxMode = false;
    unsigned int currentPlayerSelection = 0;
    unsigned int currentBackgroundSelection = 0;

};
