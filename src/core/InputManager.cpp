#include "core/InputManager.h"

std::unordered_map<int, bool> InputManager::currentKeys;
std::unordered_map<int, bool> InputManager::previousKeys;

void InputManager::init(GLFWwindow* window) {
    glfwSetKeyCallback(window, keyCallback);
}

void InputManager::update() {
    previousKeys = currentKeys;
}

bool InputManager::isKeyPressed(int key) {
    return currentKeys[key];
}

bool InputManager::wasKeyJustPressed(int key) {
    return currentKeys[key] && !previousKeys[key];
}

bool InputManager::wasKeyJustReleased(int key) {
    return !currentKeys[key] && previousKeys[key];
}

void InputManager::keyCallback(GLFWwindow*, int key, int, int action, int) {
    if (action == GLFW_PRESS) currentKeys[key] = true;
    else if (action == GLFW_RELEASE) currentKeys[key] = false;
}
