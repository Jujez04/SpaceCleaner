#pragma once
#include "GLFW/glfw3.h"

#ifndef WINDOW_H
#define WINDOW_H

class Window {
public:
	Window(int width, int height);
	~Window();
	virtual bool windowIsOpen();
	virtual void pollEvents();
private:
	virtual void createWindow(int width, int height);
	GLFWwindow* window;
};

#endif
