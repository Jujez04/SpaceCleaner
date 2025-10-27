#include "LaunchApp.h"

int main() {
	// Inizializzo GLFW
	if (!glfwInit()) {
		throw std::runtime_error("Failded to initialize GLFW");
	}
	std::unique_ptr<Engine> app = std::make_unique<Engine>();
	app->init();
	glfwTerminate();
	return 0;
}