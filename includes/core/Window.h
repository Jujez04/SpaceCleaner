#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <functional>

class Window {
public:
    Window(int width, int height);
    ~Window();

    void createWindow(int width, int height);
    bool windowIsOpen();
    void updateWindow();
    void pollEvents();
    void close();

    GLFWwindow* getWindowReference() const { return window; }

    int getWidth() const { return width; }
    int getHeight() const { return height; }

    void setResizeCallback(std::function<void(int, int)> callback);
    void setSize(int w, int h);

    void setShouldClose(bool value);
private:
    GLFWwindow* window = nullptr;
    int width = 0;
    int height = 0;

    std::function<void(int, int)> resizeCallback;
    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};
