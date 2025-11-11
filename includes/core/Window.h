#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <functional>

/**
 * @class Window
 * @brief Wrapper RAII per finestre GLFW con supporto OpenGL
 */
class Window {
public:

    /**
     * @brief Costruttore: crea la finestra con le dimensioni specificate
     * @param width Larghezza della finestra in pixel
     * @param height Altezza della finestra in pixel
     * @throws std::runtime_error Se la creazione della finestra fallisce
     */
    Window(int width, int height);

    /**
     * @brief Distruttore: distrugge automaticamente la finestra (RAII)
     *
     * Chiama close() per rilasciare le risorse GLFW.
     * Pattern RAII: garantisce cleanup anche in caso di eccezioni.
     */
    ~Window();

    /**
     * @brief Crea la finestra GLFW e inizializza OpenGL
     * @param width Larghezza in pixel
     * @param height Altezza in pixel
     * @throws std::runtime_error Se GLFW o GLAD falliscono
     */
    void createWindow(int width, int height);

    /**
     * @brief Controlla se la finestra è ancora aperta
     * @return true se la finestra non deve chiudersi, false altrimenti
     */
    bool windowIsOpen();

    /**
     * @brief Aggiorna la finestra (swap dei buffer)
     *
     * Esegue lo swap del front buffer e back buffer (double buffering).
     * Deve essere chiamato alla fine di ogni frame dopo il rendering.
     *
     * Pattern: Double Buffering
     * - Previene flickering durante il rendering
     * - Front buffer: quello visualizzato
     * - Back buffer: quello su cui si disegna
     */
    void updateWindow();

    /**
     * @brief Processa gli eventi della finestra (input, resize, etc.)
     * @note Deve essere chiamato all'inizio di ogni frame
     */
    void pollEvents();

    /**
     * @brief Chiude e distrugge la finestra
     */
    void close();

    /**
     * @brief Ottiene il puntatore raw alla finestra GLFW
     * @return Puntatore a GLFWwindow (può essere nullptr se chiusa)
     *
     * Utilizzato per:
     * - Passare la finestra a ImGui
     * - Interfacciarsi direttamente con API GLFW
     * - Inizializzare l'InputManager
     */
    GLFWwindow* getWindowReference() const {
        return window;
    }

    /**
     * @brief Ottiene la larghezza corrente della finestra
     * @return Larghezza in pixel
     */
    int getWidth() const {
        return width;
    }

    /**
     * @brief Ottiene l'altezza corrente della finestra
     * @return Altezza in pixel
     */
    int getHeight() const {
        return height;
    }

    /**
     * @brief Registra una callback per il resize della finestra
     * @param callback Funzione da chiamare quando la finestra viene ridimensionata
     */
    void setResizeCallback(std::function<void(int, int)> callback);

    /**
     * @brief Imposta le dimensioni interne (chiamato dal callback GLFW)
     * @param w Nuova larghezza
     * @param h Nuova altezza
     */
    void setSize(int w, int h);

    /**
     * @brief Richiede la chiusura della finestra
     * @param value true per richiedere chiusura, false per annullare
     */
    void setShouldClose(bool value);

private:
    /**
     * @brief Puntatore alla finestra GLFW
     */
    GLFWwindow* window = nullptr;

    /**
     * @brief Larghezza corrente della finestra in pixel
     */
    int width = 0;

    /**
     * @brief Altezza corrente della finestra in pixel
     */
    int height = 0;

    /**
     * @brief Callback personalizzabile per il resize
     */
    std::function<void(int, int)> resizeCallback;

    /**
     * @brief Callback statica GLFW per il resize del framebuffer
     * @param window Puntatore alla finestra GLFW
     * @param width Nuova larghezza del framebuffer
     * @param height Nuova altezza del framebuffer
     */
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};
