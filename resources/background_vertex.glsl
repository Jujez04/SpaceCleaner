#version 330 core
layout (location = 0) in vec2 aPos; // Coordinate di posizione del vertice

out vec2 vUV; // Variabile passata al Fragment Shader (coordinate del quad)

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // 1. Trasformazione standard del vertice
    // Questo sposta il quad nello spazio 3D e lo proietta sullo schermo.
    gl_Position = projection * view * model * vec4(aPos, 0.0, 1.0);
    
    // Queste coordinate servono al Fragment Shader per calcolare il rumore/stelle
    // in modo coerente indipendentemente dalla posizione del quad sulla scena.
    vUV = aPos; 
}