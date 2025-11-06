#include "core/Window.h"
#include <glad/glad.h>
#include <stdexcept>

// Callback GLFW per il resize
void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);

    // Recupera il puntatore alla classe Window associata
    auto win = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
    if (win) {
        win->setSize(width, height);
        if (win->resizeCallback)
            win->resizeCallback(width, height);
    }

    
}

Window::Window(int width, int height) : width(width), height(height) {
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
    glfwSetWindowUserPointer(window, this);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize GLAD");
    }

    glViewport(0, 0, width, height);
}

void Window::setResizeCallback(std::function<void(int, int)> callback) {
    resizeCallback = std::move(callback);
}

void Window::setSize(int w, int h) {
    width = w;
    height = h;
}

bool Window::windowIsOpen() {
    return !glfwWindowShouldClose(window);
}

void Window::updateWindow() {
    glfwSwapBuffers(window);
}

void Window::pollEvents() {
    glfwPollEvents();
}

void Window::close() {
    if (window) { // Add null check
        glfwDestroyWindow(window);
        window = nullptr; // Set to nullptr to prevent further access
    }
}

void Window::setShouldClose(bool value) {
    if (window)
        glfwSetWindowShouldClose(window, value ? GLFW_TRUE : GLFW_FALSE);
}