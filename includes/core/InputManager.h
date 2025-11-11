#pragma once
#include <GLFW/glfw3.h>
#include <unordered_map>

/**
*  @brief Classe per la gestione dell'input da tastiera.
*/
class InputManager {
public:
	// Inizializza il gestore di input con la finestra GLFW come contesto a cui applicare i comandi.
    static void init(GLFWwindow* window);
	// Aggiorna lo stato delle chiavi, da chiamare una volta per frame.
	// Deve essere chiamato dopo la gestione degli eventi di GLFW.

    static void update();

    static bool isKeyPressed(int key);
    static bool wasKeyJustPressed(int key);
    static bool wasKeyJustReleased(int key);

private:
    static std::unordered_map<int, bool> currentKeys;
    static std::unordered_map<int, bool> previousKeys;

    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
};
