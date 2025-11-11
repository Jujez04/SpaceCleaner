#pragma once
#include <glm/glm.hpp>
#include "graphics/Mesh.h"

// Implementazione Axis-Aligned Bounding Box (AABB) per il rilevamento delle collisioni

/**
* @brief Struttura Axis-Aligned Bounding Box (AABB)
 * Rappresenta un rettangolo allineato agli assi per il rilevamento delle collisioni 2D.
 * @param min Punto minimo (angolo in basso a sinistra)
 * @param max Punto massimo (angolo in alto a destra)
 */
struct AABB {
    glm::vec2 min;
    glm::vec2 max;

    /**
	* @brief Costruttore di default che inizializza l'AABB a valori estremi.
    */
    AABB()
        : min(std::numeric_limits<float>::max()),
        max(std::numeric_limits<float>::lowest()) {
    }   

    /**
	* @brief Costruttore che inizializza l'AABB con i punti minimi e massimi specificati.
	* Viene utilizzata per le entità con bounding box predefinite.
    */
    AABB(const glm::vec2& minPt, const glm::vec2& maxPt)
        : min(minPt), max(maxPt) {
    }

    /**
	* @brief Controlla se questa AABB collide con un'altra AABB.
    */
    bool isColliding(const AABB& other) const {
        return (min.x <= other.max.x && max.x >= other.min.x &&
            min.y <= other.max.y && max.y >= other.min.y);
    }
};

/**
* @brief Calcola l'AABB di una mesh trasformata.
* @param mesh Puntatore condiviso alla mesh
* @param transformMatrix Matrice di trasformazione da applicare alla mesh
*/
AABB calculateMeshAABB(const std::shared_ptr<Mesh>& mesh, const glm::mat4& transformMatrix);