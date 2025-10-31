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

    float aspectRatio = width / height;

    // Proiezione Orto: Mappa il centro dello schermo a (0,0).
    // Copre da -aspectRatio a +aspectRatio su X
    return glm::ortho(-1.0f * aspectRatio, 1.0f * aspectRatio,
        -1.0f, 1.0f,
        -1.0f, 1.0f);
}