#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/**
 * @brief Implementazione di una camera ortografica 2D.
 *
 * La camera gestisce la proiezione e la vista della scena, fornendo le matrici
 * necessarie per convertire le coordinate da spazio mondo a spazio vista.
 *
 * In questo caso si usa una proiezione ortografica, ideale per giochi 2D.
 */
class Camera {
public:
    /**
     * @brief Costruttore della camera.
     * @param screenWidth  Larghezza dello schermo in pixel
     * @param screenHeight Altezza dello schermo in pixel
     */
    Camera(float screenWidth, float screenHeight);

    /**
     * @brief Imposta la posizione della camera nel mondo.
     * @param pos Nuova posizione nel piano XY
     */
    void setPosition(const glm::vec2& pos);

    /**
     * @brief Reimposta la matrice di proiezione ortografica in base alle dimensioni.
     * @param width  Nuova larghezza del viewport
     * @param height Nuova altezza del viewport
     */
    void setProjection(int width, int height);

    /**
     * @brief Restituisce la matrice di vista.
     *
     * La matrice di vista rappresenta l’inverso della trasformazione della camera.
     * In pratica, sposta la scena nella direzione opposta rispetto alla posizione della camera.
     */
    glm::mat4 getViewMatrix() const;

    /**
     * @brief Restituisce la matrice di proiezione ortografica.
     *
     * La proiezione definisce il volume visibile e come le coordinate 3D vengono mappate sullo schermo.
     */
    glm::mat4 getProjectionMatrix() const;

    /// Restituisce la posizione corrente della camera
    const glm::vec2& GetPosition() const { return position; }

    /// Restituisce la larghezza della camera
    float getWidth() const { return width; }

    /// Restituisce l’altezza della camera
    float getHeight() const { return height; }

private:
    glm::vec2 position;   ///< Posizione della camera nel mondo
    float width;          ///< Larghezza dello schermo
    float height;         ///< Altezza dello schermo
    glm::mat4 projection; ///< Matrice di proiezione ortografica
};
