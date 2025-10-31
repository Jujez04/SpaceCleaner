#include "graphics/Camera.h"

Camera::Camera(float screenWidth, float screenHeight)
    : position(0.0f), width(screenWidth), height(screenHeight) {
}

void Camera::setPosition(const glm::vec2& pos) {
    position = pos;
}
glm::mat4 Camera::getViewMatrix() const {
    glm::mat4 view(1.0f);
    // Trasla la scena in direzione opposta alla posizione della camera
    return glm::translate(view, glm::vec3(-position, 0.0f));
}

glm::mat4 Camera::getProjectionMatrix() const {
    // Coordinate ortografiche con origine in basso a sinistra
    return glm::ortho(0.0f, width, 0.0f, height, -1.0f, 1.0f);
}