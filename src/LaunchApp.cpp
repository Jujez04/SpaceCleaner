#include "LaunchApp.h"

/**
* @brief Punto di ingresso principale dell'applicazione.
* Inizializza GLFW e crea un'istanza del motore di gioco.
*/
int main() {
	// Inizializzo GLFW
	if (!glfwInit()) {
		throw std::runtime_error("Failded to initialize GLFW");
	}
	std::unique_ptr<Engine> app = std::make_unique<Engine>();
	app->init();
	app->gameLoop();
	glfwTerminate();
	return 0;
}