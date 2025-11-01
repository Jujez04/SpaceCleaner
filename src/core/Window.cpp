#include "core/Window.h"
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include <stdexcept>

static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

Window::Window(int width, int height) {
	createWindow(width, height);
}

Window::~Window() {
    close();
}

void Window::createWindow(int width, int height) {
    window = glfwCreateWindow(width, height, "Space Cleaner", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize GLAD");
    }
}

bool Window::windowIsOpen() {
	return !glfwWindowShouldClose(window);
}

void Window::updateWindow() {
	glfwSwapBuffers(this->window);
}

void Window::pollEvents() {
	glfwPollEvents();
}

void Window::close() {
    glfwDestroyWindow(window);
}
