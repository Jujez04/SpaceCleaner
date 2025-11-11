#pragma once
#include <GLFW/glfw3.h>
#include <thread>
#include <chrono>

namespace Timer {

    static constexpr double TARGET_FPS = 60.0;
    static constexpr double FRAME_DURATION = 1.0 / TARGET_FPS; // 0.0166667 s (16.67 ms)

    static double totalTime = 0.0;
    static double lastFrameTime = 0.0;
    static float deltaTime = 0.0f;

    /**
     * @brief Inizializza il timer.
     */
    inline void init() {
        lastFrameTime = glfwGetTime();
        totalTime = 0.0;
        deltaTime = 0.0f;
    }

    /**
     * @brief Aggiorna il timer e blocca il frame per mantenere 60 FPS.
     *
     * Chiama questa funzione all'inizio di ogni iterazione del game loop.
     * Restituisce il tempo effettivo passato (deltaTime).
     */
    inline void update() {
        double currentTime = glfwGetTime();
        double elapsed = currentTime - lastFrameTime;

        // Se il frame è più veloce del previsto, aspetta per arrivare a 60 FPS
        if (elapsed < FRAME_DURATION) {
            double sleepTime = FRAME_DURATION - elapsed;
            std::this_thread::sleep_for(std::chrono::duration<double>(sleepTime));
            currentTime = glfwGetTime();
            elapsed = currentTime - lastFrameTime;
        }

        lastFrameTime = currentTime;
        totalTime += elapsed;
        deltaTime = static_cast<float>(elapsed);
    }

    /**
     * @brief Restituisce il tempo trascorso tra due frame consecutivi.
     */
    inline float getDeltaTime() {
        return deltaTime;
    }

    /**
     * @brief Restituisce il tempo totale trascorso dall'avvio in secondi.
     */
    inline float getTotalTime() {
        return static_cast<float>(totalTime);
    }

    /**
     * @brief Restituisce il frame rate effettivo (per debug).
     */
    inline float getCurrentFPS() {
        return 1.0f / deltaTime;
    }

}
