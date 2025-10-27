#include "LaunchApp.h"

int main() {
	// Inizializzo GLFW
	if (!glfwInit()) {
		throw std::runtime_error("Failded to initialize GLFW");
	}
	std::unique_ptr<Application> app = std::make_unique<Application>();
	app->run();
	glfwTerminate();
	return 0;
}