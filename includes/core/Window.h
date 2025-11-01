#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>

#include "graphics/Renderer.h"

class Window {
public:
	Window(int width, int height);
	~Window();
	bool windowIsOpen();
	void pollEvents();
	void updateWindow();
	void close();

	GLFWwindow* getWindowReference() const { return window; }
private:
	virtual void createWindow(int width, int height);
	GLFWwindow* window;
	std::unique_ptr<Renderer> renderer;
};

