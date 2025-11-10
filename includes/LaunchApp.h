#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "core/Engine.h"

/**
* Header file per il lancio dell'applicazione.
* Contiene glad e GLFW che nel sorgente vengono usati per 
* caricare le versioni corrette delle specifiche di OpenGL.
* Non a caso glad viene chiamato prima di GLFW, in quanto
* GLFW dipende da glad per il caricamento delle funzioni OpenGL.
*/