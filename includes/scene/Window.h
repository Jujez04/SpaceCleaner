#pragma once
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include <memory>
#include "graphics/Renderer.h"

#ifndef WINDOW_H
#define WINDOW_H

class Window {
public:
	Window(int width, int height);
	~Window();
	virtual bool windowIsOpen();
	virtual void pollEvents();
	virtual void updateWindow();
private:
	virtual void createWindow(int width, int height);
	GLFWwindow* window;
	std::unique_ptr<Renderer> renderer;
};

#endif
