#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <functional>
#include <dependencies/imgui/imgui.h>
#include <dependencies/imgui/imgui_internal.h>
#include <dependencies/imgui/imgui_impl_glfw.h>
#include <dependencies/imgui/imgui_impl_opengl3.h>

class Engine;

/**
 * @class ImGuiManager
 * @brief Gestisce l'integrazione e il rendering di ImGui nel motore di gioco.
 *
 * Questa classe si occupa di:
 *  - inizializzare ImGui e i backend OpenGL/GLFW;
 *  - gestire il frame ImGui (inizio/fine);
 *  - disegnare finestre specifiche: editor, pause, game over e start menu;
 *  - memorizzare gli stati dell'interfaccia come wireframe o selezione giocatore.
 */
class ImGuiManager {
public:
    ImGuiManager() = default;

    /**
     * @brief Costruttore principale
     * @param window Puntatore alla finestra GLFW su cui renderizzare ImGui.
     */
    ImGuiManager(GLFWwindow* window);

    /**
     * @brief Distruttore: pulisce le risorse di ImGui.
     */
    ~ImGuiManager();

    /**
     * @brief Inizia un nuovo frame ImGui. Da chiamare all’inizio del ciclo di rendering.
     */
    void beginFrame();

    /**
     * @brief Termina il frame ImGui e esegue il rendering dei dati GUI.
     */
    void endFrame();

    /** @brief Disegna la finestra editor per le impostazioni in tempo reale. */
    void drawEditorWindow(Engine* engine);

    /** @brief Disegna il menu di pausa. */
    void drawPauseMenu(Engine* engine);

    /** @brief Disegna il menu Game Over. */
    void drawGameOverMenu(Engine* engine);

    /** @brief Disegna il menu iniziale/start menu. */
    void drawStartMenu(Engine* engine);

    // Stati modificabili dall'interfaccia
    bool wireframeMode = false;            ///< Modalità wireframe per il rendering
    bool boundingBoxMode = false;          ///< Mostra le bounding box
    unsigned int currentPlayerSelection = 0; ///< Indice del player selezionato
    unsigned int currentBackgroundSelection = 0; ///< Indice dello shader/background selezionato
};
