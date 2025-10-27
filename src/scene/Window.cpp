#include "scene/Window.h"
#include "GLFW/glfw3.h"
#include <stdexcept>

Window::Window(int width, int height) {
	createWindow(width, height);
}

Window::~Window() {
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Window::createWindow(int width, int height) {
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	window = glfwCreateWindow(width, height, "Space Cleaner", nullptr, nullptr);
	if (!window) {
		glfwTerminate();
		throw std::runtime_error("Failed to create GLFW window");
	}

	glfwMakeContextCurrent(window);
	glViewport(0, 0, width, height);
}

bool Window::windowIsOpen() {
	return !glfwWindowShouldClose(window);
}

void Window::pollEvents() {
	glfwPollEvents();
}
