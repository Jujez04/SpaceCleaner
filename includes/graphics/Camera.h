#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    Camera(float screenWidth, float screenHeight);

    void setPosition(const glm::vec2& pos);
    void setProjection(int width, int height);

    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;

    const glm::vec2& GetPosition() const { return position; }

    float getWidth() const { return width; }
    float getHeight() const { return height; }

private:
    glm::vec2 position;
    float width;
    float height;

    glm::mat4 projection = glm::mat4(1.0f);
};
