#pragma once
#include <string>
#include <glm/glm.hpp>

struct SubMeshInfo {
    std::string name;             // Nome del file .txt o della forma
    glm::vec4 color;              // Colore del layer
    glm::mat4 localTransform;     // Trasformazione (scala, offset)
};

struct PlayerConfig {
    std::string name;             // Nome del modello (es. "X-Wing", "Faucon")
    SubMeshInfo base;
    SubMeshInfo livery;
    SubMeshInfo cockpit;
    // ID delle mesh caricate dopo l'inizializzazione:
    unsigned int baseMeshId = 0;
    unsigned int liveryMeshId = 0;
    unsigned int cockpitMeshId = 0;
};