#include "graphics/Camera.h"

Camera::Camera(float screenWidth, float screenHeight)
    : position(0.0f), width(screenWidth), height(screenHeight) {
    setProjection(static_cast<int>(screenWidth), static_cast<int>(screenHeight));
}

void Camera::setPosition(const glm::vec2& pos) {
    position = pos;
}

void Camera::setProjection(int width, int height) {
    this->width = static_cast<float>(width);
    this->height = static_cast<float>(height);

    float aspectRatio = width / static_cast<float>(height);
    projection = glm::ortho(-aspectRatio, aspectRatio, -1.0f, 1.0f, -1.0f, 1.0f);
}

glm::mat4 Camera::getViewMatrix() const {
    glm::mat4 view(1.0f);
    return glm::translate(view, glm::vec3(-position, 0.0f));
}

glm::mat4 Camera::getProjectionMatrix() const {
    return projection;
}
