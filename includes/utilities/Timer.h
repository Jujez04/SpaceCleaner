#pragma once

#include <GLFW/glfw3.h>

namespace Timer {

    static double totalTime = 0.0;
    static float deltaTime = 0.0f;
    static double lastFrameTime = 0.0;

    /**
     * @brief Inizializza o ripristina il timer.
     * Dovrebbe essere chiamato una sola volta all'avvio dell'applicazione.
     */
    inline void init() {
        lastFrameTime = glfwGetTime();
        totalTime = 0.0;
        deltaTime = 0.0f;
    }

    /**
     * @brief Calcola deltaTime e aggiorna totalTime.
     * DEVE essere chiamato all'inizio di ogni ciclo di gioco (game loop).
     */
    inline void update() {
        double currentFrameTime = glfwGetTime();
        deltaTime = static_cast<float>(currentFrameTime - lastFrameTime);
        lastFrameTime = currentFrameTime;

        totalTime += deltaTime;
    }

    /**
     * @brief Restituisce il tempo trascorso in secondi dall'ultimo frame.
     */
    inline float getDeltaTime() {
        return deltaTime;
    }

    /**
     * @brief Restituisce il tempo totale trascorso dall'avvio in secondi.
     */
    inline float getTotalTime() {
        // Restituisce un float per uniformità con deltaTime nello shader
        return static_cast<float>(totalTime);
    }
}