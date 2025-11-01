#pragma once
#include <GLFW/glfw3.h>
#include <unordered_map>

class InputManager {
public:
    static void init(GLFWwindow* window);
    static void update();

    static bool isKeyPressed(int key);
    static bool wasKeyJustPressed(int key);
    static bool wasKeyJustReleased(int key);

private:
    static std::unordered_map<int, bool> currentKeys;
    static std::unordered_map<int, bool> previousKeys;

    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
};
