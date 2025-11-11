#include "graphics/Camera.h"

Camera::Camera(float screenWidth, float screenHeight)
    : position(0.0f), width(screenWidth), height(screenHeight)
{
    // Inizializza la matrice di proiezione con le dimensioni dello schermo
    setProjection(static_cast<int>(screenWidth), static_cast<int>(screenHeight));
}

void Camera::setPosition(const glm::vec2& pos)
{
    position = pos;
}

void Camera::setProjection(int width, int height)
{
    this->width = static_cast<float>(width);
    this->height = static_cast<float>(height);

    // Calcola l'aspect ratio e genera una proiezione ortografica centrata
    float aspectRatio = width / static_cast<float>(height);
    projection = glm::ortho(-aspectRatio, aspectRatio, -1.0f, 1.0f, -1.0f, 1.0f);
}

glm::mat4 Camera::getViewMatrix() const
{
    glm::mat4 view(1.0f);

    // La vista è ottenuta traslando la scena nella direzione opposta alla camera
    return glm::translate(view, glm::vec3(-position, 0.0f));
}

glm::mat4 Camera::getProjectionMatrix() const
{
    return projection;
}
